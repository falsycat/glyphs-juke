#pragma once

#include "common.h"
#include "iAllocator.h"
#include "iAudioDevice.h"
#include "iElementFactory.h"
#include "MusicElement.h"


namespace gj {

class MusicElementFactory : public iElementFactory {
 public:
  MusicElementFactory(MusicElementFactory&&) = delete;
  MusicElementFactory(const MusicElementFactory&) = delete;

  MusicElementFactory& operator=(MusicElementFactory&&) = delete;
  MusicElementFactory& operator=(const MusicElementFactory&) = delete;

  MusicElementFactory(iAllocator* alloc, iAudioDevice* audio) :
      alloc_(alloc), audio_(audio) {
  }

  UniqPtr<iElement> Create(Param&& param) override {
    if (param.custom.size() != 2) return nullptr;

    const std::string path   = std::get<std::string>(param.custom[0]);
    const double      offset = std::get<double>(param.custom[1]);

    MusicElement::Param p;
    p.audio  = audio_;
    p.period = param.period;
    p.path   = path;
    p.offset = offset;
    p.driver = std::move(param.driver);
    return alloc_->MakeUniq<iElement, MusicElement>(std::move(p));
  }

 private:
  iAllocator*   alloc_;
  iAudioDevice* audio_;
};


}