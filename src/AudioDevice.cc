#define MINIAUDIO_IMPLEMENTATION
#include "AudioDevice.h"

#include "common.h"


gj::AudioDevice::AudioDevice() {
  ma_device_config config = ma_device_config_init(ma_device_type_playback);
  config.playback.format   = kFormat;
  config.playback.channels = kChannel;
  config.sampleRate        = kSampleRate;
  config.dataCallback      = Callback_;
  config.pUserData         = this;

  if (ma_device_init(nullptr, &config, &ma_) != MA_SUCCESS) {
    Abort("AudioDevice error");
  }
  ma_device_start(&ma_);
}

gj::AudioDevice::~AudioDevice() {
  ma_device_stop(&ma_);
  ma_device_uninit(&ma_);

  /* the worker thread has been exited so no lock is necessary anymore */

  if (playing_) {
    ma_decoder_uninit(&dec_);
  }
}

void gj::AudioDevice::PlayMusic(const std::string& path) {
  std::lock_guard _(mtx_);

  if (playing_) {
    ma_decoder_uninit(&dec_);
  }
  ma_decoder_config config = ma_decoder_config_init(kFormat, kChannel, kSampleRate);
  if (ma_decoder_init_file(path.c_str(), &config, &dec_) != MA_SUCCESS) {
    Abort("AudioDevice decoder error: "+path);
  }
  playing_ = true;
}

void gj::AudioDevice::StopMusic() {
  std::lock_guard _(mtx_);
  if (!playing_) return;

  ma_decoder_uninit(&dec_);
  playing_ = false;
}

void gj::AudioDevice::Callback_(ma_device* ma, void* out, const void* in, ma_uint32 framecnt) {
  AudioDevice* dev = reinterpret_cast<AudioDevice*>(ma->pUserData);
  std::lock_guard _(dev->mtx_);

  float* dst = reinterpret_cast<float*>(out);

  size_t wrote = 0;
  if (dev->playing_) {
    const ma_uint64 n = ma_decoder_read_pcm_frames(&dev->dec_, dst, framecnt);
    if (n < framecnt) {
      dev->playing_ = false;
      ma_decoder_uninit(&dev->dec_);
    }
    wrote += n;
  }
  dev->time_.fetch_add(framecnt);
}