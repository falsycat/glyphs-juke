#pragma once

#include "common.h"
#include "HiraganaMatcher.h"
#include "iAllocator.h"
#include "iClock.h"
#include "iElementFactory.h"
#include "InputWindowElement.h"


namespace gj {


class InputWindowElementFactory : public iElementFactory {
 public:
  InputWindowElementFactory(InputWindowElementFactory&&) = delete;
  InputWindowElementFactory(const InputWindowElementFactory&) = delete;

  InputWindowElementFactory& operator=(InputWindowElementFactory&&) = delete;
  InputWindowElementFactory& operator=(const InputWindowElementFactory&) = delete;

  InputWindowElementFactory(iAllocator* alloc, Scoreboard* sb) : alloc_(alloc), sb_(sb) {
  }

  UniqPtr<iElement> Create(Param&& param) override {
    if (param.custom.size() != 2) return nullptr;

    const std::string text = std::get<std::string>(param.custom[0]);
    const std::string kana = std::get<std::string>(param.custom[1]);

    InputWindowElement::Param p;
    p.period     = param.period;
    p.scoreboard = sb_;
    p.driver     = std::move(param.driver);
    p.matcher    = alloc_->MakeUniq<HiraganaMatcher>(ConvertStrToWstr(kana));
    p.text       = ConvertStrToWstr(text);

    return alloc_->MakeUniq<InputWindowElement>(std::move(p));
  }

 private:
  iAllocator* alloc_;
  Scoreboard* sb_;
};


}