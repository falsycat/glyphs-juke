#pragma once

#include <cstdint>

#include "Frame.h"
#include "iAllocator.h"
#include "iAudioDevice.h"
#include "iClock.h"
#include "iDrawable.h"
#include "iWritable.h"


namespace gj {


class iScene {
 public:
  struct Param {
    iAllocator*   alloc;
    iAudioDevice* audio;
    const iClock* clock;
  };

  iScene() = default;
  iScene(iScene&&) = default;
  iScene(const iScene&) = default;

  iScene& operator=(iScene&&) = default;
  iScene& operator=(const iScene&) = default;

  virtual ~iScene() = default;

  /* Returns next scene if this scene ends, otherwise nullptr. */
  virtual UniqPtr<iScene> Update(Frame& f) = 0;
};


}