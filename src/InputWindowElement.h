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
    iAllocator* alloc;

    UniqPtr<iInputMatcher>  matcher;
    UniqPtr<iElementDriver> driver;

    Scoreboard* scoreboard;

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
      bg_(CreateEmptySquare_(p.alloc)), text_(p.text), expects_(matcher_->expects()) {
    param_["posX"]  = 0.;
    param_["posY"]  = 0.;
    param_["baseX"] = .5;
  }

  void Update(Frame& frame, double t) override {
    /* consumes all input */
    for (auto c : frame.input) {
      if (matcher_->done()) break;
      if (matcher_->Input(c)) {
        expects_ = Text(matcher_->expects());
        ++sb_->correct;
      }
      ++sb_->input;
    }

    /* executes the driver to update parameters and takes them with cast */
    drv_->Update(param_, t);

    const auto posX  = std::get<double>(param_["posX"]);
    const auto posY  = std::get<double>(param_["posY"]);
    const auto baseX = std::get<double>(param_["baseX"]);

    const uint32_t posXi  = static_cast<int32_t>((posX+1)/2 * frame.w);
    const uint32_t posYi  = static_cast<int32_t>((posY+1)/2 * frame.h);

    const uint32_t tbaseXi = static_cast<int32_t>(baseX * text_.width());
    const uint32_t ebaseXi = static_cast<int32_t>(baseX * expects_.width());

    const double w = (4+std::max(text_.width(), expects_.width()))*1. / frame.w;
    const double h = 4. / frame.h;

    /*  calculates geometry of background rect
     * and sets alpha to negative to decrease colors of pixels surrounding the text */
    auto M = mat3{
      {w, 0, 0},
      {0, h, 0},
      {0, 0, 1},
    };
    M = ::linalg::mul(mat3{
      {1, 0, 0},
      {0, 1, 0},
      {posX, posY, 1},
    }, M);
    bg_.SetMatrix(M);
    bg_.SetAlpha(-.8f);
    frame.Add(&bg_);

    /* calculates geometry of foreground text */
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

  bool HasPrepared() const override {
    return true;
  }

 private:
  UniqPtr<iInputMatcher>  matcher_;
  UniqPtr<iElementDriver> drv_;

  Scoreboard* sb_;

  Texture bg_;
  Text    text_;
  Text    expects_;

  iElementDriver::Param param_;

  static Colorbuffer CreateEmptySquare_(iAllocator* alloc) {
    auto buf = Colorbuffer(alloc, 1, 1);
    buf.ptr()[0] = 1;
    return std::move(buf);
  }
};


}