#pragma once

#include <atomic>
#include <thread>

#define NOMINMAX
#include "thirdparty/miniaudio.h"
#undef NOMINMAX

#include "common.h"
#include "iAudioEffect.h"


namespace gj {


class Music : public iAudioEffect {
 public:
  static constexpr auto kVolumeLpf = .9f;

  Music() = delete;
  Music(Music&&) = delete;
  Music(const Music&) = delete;

  Music& operator=(Music&&) = delete;
  Music& operator=(const Music&) = delete;

  Music(const std::string& path, uint8_t ch, uint32_t srate) :
      ch_(ch), srate_(srate), lpf_prev_(ch) {
    const auto config = ma_decoder_config_init(ma_format_f32, ch, srate);

    if (ma_decoder_init_file(path.c_str(), &config, &dec_)) {
      Abort("decoder error: "+path);
    }
  }
  ~Music() {
    if (seeker_.joinable()) {
      seeker_.join();
    }
    ma_decoder_uninit(&dec_);
  }

  void Play() {
    playing_.store(true);
  }
  void Stop() {
    playing_.store(false);
  }
  void Seek(double sec) {
    if (seeker_.joinable()) {
      seeker_.join();
    }
    seeking_.store(true);

    /* executes seek function in new thread */
    const size_t frame = static_cast<size_t>(srate_*sec);
    seeker_ = std::thread([this, frame]() {
      if (ma_decoder_seek_to_pcm_frame(&dec_, frame) != MA_SUCCESS) {
        Abort("decoder seek failure");
      }
      seeking_.store(false);
    });
  }

  void SetVolume(float f) {
    volume_.store(f);
  }
  void SetLpf(float f) {
    lpf_.store(f);
  }

  bool IsBusy() const {
    return seeking_.load();
  }

  void Apply(float* ptr, size_t frames) override {
    if (seeking_.load() || !playing_.load()) return;

    frames = ma_decoder_read_pcm_frames(&dec_, ptr, frames);

    const float lpf    = lpf_.load();
    const float lpfinv = 1-lpf;

    const float volume = volume_.load();

    /* applies volume and LPF */
    const size_t n = frames * ch_;
    for (size_t i = 0; i < frames; ++i) {
      volume_actual_ = volume_actual_*kVolumeLpf + volume*(1-kVolumeLpf);

      float* sample = &ptr[i*ch_];
      for (uint8_t j = 0; j < ch_; ++j) {
        sample[j]    = lpf_prev_[j]*lpf + sample[j]*volume_actual_*lpfinv;
        lpf_prev_[j] = sample[j];
      }
    }
  }

 private:
  uint8_t  ch_;
  uint32_t srate_;

  std::thread seeker_;

  ma_decoder dec_{0};

  std::atomic<bool> seeking_ = false;
  std::atomic<bool> playing_ = false;

  std::atomic<float> volume_ = 0;
  std::atomic<float> lpf_    = 0;

  float volume_actual_;
  std::vector<float> lpf_prev_;
};


}