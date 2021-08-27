#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>

#define NOMINMAX  /* miniaudio includes windows.h */
#include "thirdparty/miniaudio.h"
#undef NOMINMAX

#include "iAudioDevice.h"
#include "iClock.h"


namespace gj {


class AudioDevice : public iAudioDevice, public iClock {
 public:
  static constexpr auto kFormat     = ma_format_f32;
  static constexpr auto kChannel    = 2;
  static constexpr auto kSampleRate = 48000;

  AudioDevice(AudioDevice&&) = default;
  AudioDevice(const AudioDevice&) = default;

  AudioDevice& operator=(AudioDevice&&) = default;
  AudioDevice& operator=(const AudioDevice&) = default;

  AudioDevice();
  ~AudioDevice();

  void PlayMusic(const std::string& path, double offset) override;
  void StopMusic() override;

  void SetVolume(double amp) override {
    amp_.store(amp);
  }
  void SetLpfIntensity(double v) override {
    lpf_coe_.store(v);
  }

  uint64_t now() const override {
    return time_.load() * 1000 / kSampleRate;
  }

 private:
  std::mutex mtx_;

  ma_device ma_{0};

  bool playing_ = false;
  ma_decoder dec_{0};

  std::atomic<double> amp_ = 1;

  std::atomic<double> lpf_coe_  = 0;
  float               lpf_prev_ = 0;

  std::atomic<uint64_t> time_;

  static void Callback_(ma_device* ma, void* out, const void* in, ma_uint32 framecnt);
};


}