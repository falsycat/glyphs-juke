#pragma once

#include <cmath>

#include "iAudioDevice.h"
#include "iElement.h"
#include "iElementDriver.h"
#include "Texture.h"

namespace gj {


class MusicElement : public iElement {
public:
  struct Param {
    iAudioDevice* audio;

    Period period;

    std::string path;
    double      offset;

    UniqPtr<iElementDriver> driver;
  };

  MusicElement() = delete;
  MusicElement(MusicElement&&) = delete;
  MusicElement(const MusicElement&) = delete;

  MusicElement& operator=(MusicElement&&) = delete;
  MusicElement& operator=(const MusicElement&) = delete;

  MusicElement(Param&& p) :
      iElement(p.period), audio_(p.audio), path_(p.path), offset_(p.offset),
      drv_(std::move(p.driver)) {
    param_["volume"] = 0.;
    param_["lpf"]    = 0.;
  }

  void Update(Frame& frame, double t) override {
    drv_->Update(param_, t);

    const double volume = std::get<double>(param_["volume"]);
    const double lpf    = std::get<double>(param_["lpf"]);

    audio_->SetVolume(volume);
    audio_->SetLpfIntensity(lpf);

    if (first_) {
      audio_->PlayMusic(path_, offset_);
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

  double offset_;

  UniqPtr<iElementDriver> drv_;
  iElementDriver::Param   param_;
};


}