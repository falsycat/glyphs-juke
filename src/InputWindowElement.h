#pragma once

#include <cstdint>

#include "common.h"
#include "iElement.h"
#include "iElementDriver.h"
#include "iInputMatcher.h"
#include "Scoreboard.h"
#include "Text.h"

namespace gj {


class InputWindowElement : public iElement {
 public:
  struct Param {
    UniqPtr<iInputMatcher>  matcher;
    UniqPtr<iElementDriver> driver;

    Scoreboard* scoreboard = nullptr;

    Period period;

    std::wstring text;
  };

  InputWindowElement() = delete;
  InputWindowElement(InputWindowElement&&) = delete;
  InputWindowElement(const InputWindowElement&) = delete;

  InputWindowElement& operator=(InputWindowElement&&) = delete;
  InputWindowElement& operator=(const InputWindowElement&) = delete;

  InputWindowElement(Param&& p) :
      iElement(p.period),
      matcher_(std::move(p.matcher)), drv_(std::move(p.driver)), sb_(p.scoreboard),
      text_(p.text), expects_(matcher_->expects()) {
    param_["posX"]  = 0.;
    param_["posY"]  = 0.;
    param_["baseX"] = .5;
  }

  void Update(Frame& frame, double t) override {
    for (auto c : frame.input) {
      if (matcher_->done()) break;
      if (matcher_->Input(c)) {
        expects_ = Text(matcher_->expects());
        ++sb_->correct;
      }
      ++sb_->input;
    }
    drv_->Update(param_, t);

    const auto posX  = (std::get<double>(param_["posX"])+1)/2;
    const auto posY  = (std::get<double>(param_["posY"])+1)/2;
    const auto baseX = std::get<double>(param_["baseX"]);

    const uint32_t posXi  = static_cast<int32_t>(posX * frame.w);
    const uint32_t posYi  = static_cast<int32_t>(posY * frame.h);

    const uint32_t tbaseXi = static_cast<int32_t>(baseX * text_.width());
    const uint32_t ebaseXi = static_cast<int32_t>(baseX * expects_.width());

    text_.SetPosition(posXi-tbaseXi, posYi);
    expects_.SetPosition(posXi-ebaseXi, posYi+1);
    frame.Add(&text_);
    frame.Add(&expects_);
  }

  void Finalize() override {
    ++sb_->lines;
    if (matcher_->done()) {
      ++sb_->completeLines;
    }
  }

 private:
  UniqPtr<iInputMatcher>  matcher_;
  UniqPtr<iElementDriver> drv_;

  Scoreboard* sb_;

  Text text_;
  Text expects_;

  iElementDriver::Param param_;
};


}