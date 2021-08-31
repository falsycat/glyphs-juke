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
      /* glitch happens with 10% of chance */
      if (XorShift(seed+y)%10 == 0) continue;
      
      /* calculates how many pixels shift the line */
      const double shift = (XorShift(seed+y+h)%100/100.*2-1)*maxShift;
      if (std::abs(shift) > 1) continue;

      const int32_t s  = static_cast<int32_t>(w*shift);
      const int32_t as = std::abs(s);
      
      /* get pointers to actual data */
      float* src = ptr + static_cast<size_t>(y) * w;
      float* dst = src + as;
      if (s < 0) std::swap(src, dst);

      /* shifts pixels */
      std::memmove(dst, src, (static_cast<size_t>(w) - as)*sizeof(*ptr));
    }
  }

  uint64_t seed     = 1;
  double   maxShift = 0;
};


}