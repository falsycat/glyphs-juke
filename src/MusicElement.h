#pragma once

#include <cmath>

#include "iAudioDevice.h"
#include "iElement.h"
#include "iElementDriver.h"
#include "Music.h"

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
      iElement(p.period), audio_(p.audio),
      music_(p.path, audio_->ch(), audio_->sampleRate()),
      drv_(std::move(p.driver)) {
    music_.Seek(p.offset);
    audio_->AddEffect(&music_);

    param_["volume"] = 0.;
    param_["lpf"]    = 0.;
  }

  void Update(Frame& frame, double t) override {
    drv_->Update(param_, t);

    const double volume = std::get<double>(param_["volume"]);
    const double lpf    = std::get<double>(param_["lpf"]);

    music_.SetVolume(static_cast<float>(volume));
    music_.SetLpf(static_cast<float>(lpf));

    if (first_) {
      music_.Play();
      first_ = false;
    }
  }

  void Finalize() override {
    audio_->RemoveEffect(&music_);
  }

  bool HasPrepared() const override {
    return !music_.IsBusy();
  }

 private:
  iAudioDevice* audio_;
  Music         music_;

  bool first_ = true;

  UniqPtr<iElementDriver> drv_;
  iElementDriver::Param   param_;
};


}