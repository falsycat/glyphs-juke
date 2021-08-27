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

  virtual void PlayMusic(const std::string& path) = 0;
  virtual void StopMusic() = 0;
};


}