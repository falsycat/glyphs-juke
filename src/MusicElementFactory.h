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
    if (param.custom.size() != 1) return nullptr;

    const std::string name = std::get<std::string>(param.custom[0]);

    return alloc_->MakeUniq<iElement, MusicElement>(param.period, audio_, name);
  }

 private:
  iAllocator*   alloc_;
  iAudioDevice* audio_;
};


}