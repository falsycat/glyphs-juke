#pragma once

#include <cstdint>

#include "common.h"
#include "iElement.h"
#include "iElementDriver.h"
#include "iInputMatcher.h"
#include "Text.h"

namespace gj {


class InputWindowElement : public iElement {
 public:
  struct Param {
    UniqPtr<iInputMatcher>  matcher;
    UniqPtr<iElementDriver> driver;

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
      matcher_(std::move(p.matcher)), drv_(std::move(p.driver)),
      text_(p.text), guide_(matcher_->expects()), width_(CountWstrBytes(p.text)) {
    param_["posX"]  = 0.;
    param_["posY"]  = 0.;
    param_["baseX"] = .5;
  }

  void Update(Frame& frame, double t) override {
    for (auto c : frame.input) {
      if (matcher_->done()) break;
      if (matcher_->Input(c)) {
        guide_ = Text(matcher_->expects());
      }
    }
    drv_->Update(param_, t);

    const auto posX  = (std::get<double>(param_["posX"])+1)/2;
    const auto posY  = (std::get<double>(param_["posY"])+1)/2;
    const auto baseX = std::get<double>(param_["baseX"]);

    const uint32_t posXi  = static_cast<int32_t>(posX * frame.w);
    const uint32_t posYi  = static_cast<int32_t>(posY * frame.h);
    const uint32_t baseXi = static_cast<int32_t>(baseX * width_);

    text_.SetPosition(posXi-baseXi, posYi);
    guide_.SetPosition(posXi-baseXi, posYi+1);
    frame.Add(&text_);
    frame.Add(&guide_);
  }

  void Finalize() override {
    /* TODO score calculation */
  }

 private:
  UniqPtr<iInputMatcher>  matcher_;
  UniqPtr<iElementDriver> drv_;

  Text text_;
  Text guide_;

  size_t width_;

  iElementDriver::Param param_;
};


}