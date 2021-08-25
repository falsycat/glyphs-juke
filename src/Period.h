#pragma once

#include <cstdint>

namespace gj {


struct Period {
 public:
  Period() = delete;

  Period(uint64_t st, uint64_t ed) : start(st), end(ed) {
    assert(st <= ed);
  }

  bool isHit(uint64_t now) const {
    return start <= now && now < end;
  }

  uint64_t duration() const {
    return end - start;
  }

  uint64_t start;
  uint64_t end;
};


}