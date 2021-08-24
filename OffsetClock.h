#pragma once

#include <cstdint>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "iClock.h"

namespace gj {


class OffsetClock : public iClock {
 public:
  OffsetClock(OffsetClock&&) = default;
  OffsetClock(const OffsetClock&) = default;

  OffsetClock& operator=(OffsetClock&&) = default;
  OffsetClock& operator=(const OffsetClock&) = default;

  OffsetClock(const iClock* parent, uint64_t offset) :
      parent_(parent), offset_(offset) {
  }
  OffsetClock(const iClock* parent) : OffsetClock(parent, parent->now()) {
  }

  uint64_t now() const override {
    return parent_->now() - offset_;
  }

 private:
  const iClock* parent_;
  uint64_t      offset_;
};


}