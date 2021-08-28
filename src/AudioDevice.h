#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

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

  void AddEffect(iAudioEffect* fx) override {
    std::lock_guard _(mtx_);
    effects_.push_back(fx);
  }
  void RemoveEffect(iAudioEffect* fx) override {
    std::lock_guard _(mtx_);

    auto itr = std::find(effects_.begin(), effects_.end(), fx);
    if (itr == effects_.end()) return;
    effects_.erase(itr);
  }

  uint8_t ch() const override {
    return kChannel;
  }
  uint32_t sampleRate() const override {
    return kSampleRate;
  }

  uint64_t now() const override {
    return time_.load() * 1000 / kSampleRate;
  }

 private:
  std::mutex mtx_;

  ma_device ma_{0};

  std::atomic<uint64_t> time_;

  std::vector<iAudioEffect*> effects_;

  static void Callback_(ma_device* ma, void* out, const void* in, ma_uint32 framecnt);
};


}