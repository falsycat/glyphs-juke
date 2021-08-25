#pragma once

#include <cstdint>

namespace gj {


struct Period {
 public:
  Period() : Period(0, 0) {
  }
  Period(uint64_t st, uint64_t ed) : start(st), end(ed) {
    assert(st <= ed);
  }

  double Normalize(uint64_t now) const {
    return (static_cast<int64_t>(now) - start)*1./duration();
  }

  bool IsHit(uint64_t now) const {
    return start <= now && now < end;
  }

  uint64_t duration() const {
    return end - start;
  }

  uint64_t start;
  uint64_t end;
};


}