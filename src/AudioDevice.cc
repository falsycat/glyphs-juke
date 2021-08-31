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
}

void gj::AudioDevice::Callback_(ma_device* ma, void* out, const void* in, ma_uint32 framecnt) {
  AudioDevice* dev = reinterpret_cast<AudioDevice*>(ma->pUserData);
  std::lock_guard _(dev->mtx_);

  /* iterates all of effects and applies them to the current frame */
  for (auto fx : dev->effects_) {
    fx->Apply(reinterpret_cast<float*>(out), framecnt);
  }

  /* count up the time */
  dev->time_.fetch_add(framecnt);
}