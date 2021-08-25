#pragma once

#include <cstdint>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "iClock.h"


namespace gj {


  class TickingClock : public iClock {
  public:
    TickingClock() = delete;
    TickingClock(TickingClock&&) = default;
    TickingClock(const TickingClock&) = default;

    TickingClock& operator=(TickingClock&&) = default;
    TickingClock& operator=(const TickingClock&) = default;

    TickingClock(const iClock* parent) : parent_(parent) {
      Tick();
    }

    void Tick() {
      now_ = parent_->now();
    }

    uint64_t now() const override {
      return now_;
    }

   private:
    const iClock* parent_;

    uint64_t now_;
  };


}