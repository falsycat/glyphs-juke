#pragma once

#include <cstdint>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "iClock.h"


namespace gj {


class SystemClock : public iClock {
 public:
  static const SystemClock& instance() {
    static const SystemClock instance_;
    return instance_;
  }

  SystemClock() = default;
  SystemClock(SystemClock&&) = default;
  SystemClock(const SystemClock&) = default;

  SystemClock& operator=(SystemClock&&) = default;
  SystemClock& operator=(const SystemClock&) = default;

  uint64_t now() const override {
    return GetTickCount64();
  }
};


}