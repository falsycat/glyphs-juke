#pragma once

#include <cstdint>


namespace gj {


class iClock {
 public:
  iClock() = default;
  iClock(iClock&&) = default;
  iClock(const iClock&) = default;

  iClock& operator=(iClock&&) = default;
  iClock& operator=(const iClock&) = default;

  virtual ~iClock() = default;

  virtual uint64_t now() const = 0;
};



}