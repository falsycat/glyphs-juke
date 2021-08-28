#pragma once

#include <cstdint>


namespace gj {


/* Users have a responsible to remove AudioEffect from all devices at its destruction. */
class iAudioEffect {
 public:
  iAudioEffect() = default;
  iAudioEffect(iAudioEffect&&) = default;
  iAudioEffect(const iAudioEffect&) = default;

  iAudioEffect& operator=(iAudioEffect&&) = default;
  iAudioEffect& operator=(const iAudioEffect&) = default;

  virtual ~iAudioEffect() = default;

  virtual void Apply(float* ptr, size_t frame) = 0;
};



}