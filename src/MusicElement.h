#pragma once

#include <cmath>

#include "iAudioDevice.h"
#include "iElement.h"
#include "iElementDriver.h"
#include "Texture.h"

namespace gj {


class MusicElement : public iElement {
public:
  MusicElement() = delete;
  MusicElement(MusicElement&&) = delete;
  MusicElement(const MusicElement&) = delete;

  MusicElement& operator=(MusicElement&&) = delete;
  MusicElement& operator=(const MusicElement&) = delete;

  MusicElement(const Period& p, iAudioDevice* audio, const std::string& name) :
      iElement(p), audio_(audio), path_("res/music/"+name) {
  }

  void Update(Frame& frame, double t) override {
    if (first_) {
      audio_->PlayMusic(path_);
      first_ = false;
    }
  }

  void Finalize() override {
    if (!first_) {
      audio_->StopMusic();
    }
  }

 private:
  iAudioDevice* audio_;
  std::string   path_;

  bool first_ = true;
};


}