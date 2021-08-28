#pragma once

#include <string>

#include "iAudioEffect.h"


namespace gj {


class iAudioDevice {
public:
  iAudioDevice(iAudioDevice&&) = default;
  iAudioDevice(const iAudioDevice&) = default;

  iAudioDevice& operator=(iAudioDevice&&) = default;
  iAudioDevice& operator=(const iAudioDevice&) = default;

  iAudioDevice() = default;

  virtual ~iAudioDevice() = default;

  virtual void AddEffect(iAudioEffect* fx) = 0;
  virtual void RemoveEffect(iAudioEffect* fx) = 0;

  virtual uint8_t  ch()         const = 0;
  virtual uint32_t sampleRate() const = 0;
};


}