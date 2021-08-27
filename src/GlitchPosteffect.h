#pragma once

#include <cstring>
#include <cmath>

#include "iDrawable.h"


namespace gj {


class GlitchPosteffect : public iDrawable {
 public:
  GlitchPosteffect(GlitchPosteffect&&) = delete;
  GlitchPosteffect(const GlitchPosteffect&) = delete;

  GlitchPosteffect& operator=(GlitchPosteffect&&) = delete;
  GlitchPosteffect& operator=(const GlitchPosteffect&) = delete;
  
  GlitchPosteffect() {
  }

  void Draw(Colorbuffer& fb) const override {
    const int32_t w = static_cast<int32_t>(fb.width());
    const int32_t h = static_cast<int32_t>(fb.height());

    float* ptr = fb.ptr();

    for (int32_t y = 0; y < h; ++y) {
      const double shift = (XorShift(seed+y)%100/100.*2-1)*maxShift;
      const int32_t s = static_cast<int32_t>(w*shift);
      if (std::abs(shift) > 1) continue;
      
      const size_t offset = static_cast<size_t>(y) * w;
      float* src = ptr + offset;
      float* dst = ptr + offset + s;
      if (src > dst) std::swap(src, dst);

      std::memmove(dst, src, static_cast<size_t>(w) - std::abs(s));
    }
  }

  uint64_t seed     = 1;
  double   maxShift = 0;
};


}