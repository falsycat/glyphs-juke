#pragma once

#include <string>


namespace gj {


class iAudioDevice {
public:
  iAudioDevice(iAudioDevice&&) = default;
  iAudioDevice(const iAudioDevice&) = default;

  iAudioDevice& operator=(iAudioDevice&&) = default;
  iAudioDevice& operator=(const iAudioDevice&) = default;

  iAudioDevice() = default;

  virtual ~iAudioDevice() = default;

  virtual void PlayMusic(const std::string& path, double offset) = 0;
  virtual void StopMusic() = 0;

  virtual void SetVolume(double amp) = 0;
  virtual void SetLpfIntensity(double v) = 0;
};


}