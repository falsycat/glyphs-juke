#pragma once

#include <algorithm>

#include "thirdparty/linalg.h"

#include "iDrawable.h"
#include "Rasterbuffer.h"


namespace gj {


class Texture : public DrawableBase {
 public:
  Texture() = delete;
  Texture(Texture&&) = default;
  Texture(const Texture&) = default;

  Texture& operator=(Texture&&) = default;
  Texture& operator=(const Texture&) = default;

  Texture(Colorbuffer&& src) : src_(std::move(src)) {
  }

  void Draw(Colorbuffer& fb) const override {
    const int32_t w = static_cast<int32_t>(fb.width());
    const int32_t h = static_cast<int32_t>(fb.height());

    const int32_t srcw = static_cast<int32_t>(src_.width());
    const int32_t srch = static_cast<int32_t>(src_.height());

    vec3 p[4] = {
      { -1,  1, 1 },
      { -1, -1, 1 },
      {  1, -1, 1 },
      {  1,  1, 1 },
    };
    for (size_t i = 0; i < 4; ++i) {
      p[i] = ::linalg::mul(mat_, p[i]);
    }
    const double pl = std::min({p[0].x, p[1].x, p[2].x, p[3].x})-.1;
    const double pr = std::max({p[0].x, p[1].x, p[2].x, p[3].x})+.1;
    const double pu = std::max({p[0].y, p[1].y, p[2].y, p[3].y})+.1;
    const double pb = std::min({p[0].y, p[1].y, p[2].y, p[3].y})-.1;
    const double pw = pr - pl;
    const double ph = pu - pb;

    const int32_t pli = static_cast<int32_t>((pl + 1) / 2 * w);
    const int32_t pri = static_cast<int32_t>((pr + 1) / 2 * w);
    const int32_t pui = static_cast<int32_t>((pu + 1) / 2 * h);
    const int32_t pbi = static_cast<int32_t>((pb + 1) / 2 * h);
    const int32_t pwi = pri - pli;
    const int32_t phi = pui - pbi;

    vec3 q[4] = {
      { pl, pu, 1 },
      { pl, pb, 1 },
      { pr, pb, 1 },
      { pr, pu, 1 },
    };
    for (size_t i = 0; i < 4; ++i) {
      q[i] = ::linalg::mul(invmat_, q[i]);
    }
    const double ql = std::min({q[0].x, q[1].x, q[2].x, q[3].x});
    const double qr = std::max({q[0].x, q[1].x, q[2].x, q[3].x});
    const double qu = std::max({q[0].y, q[1].y, q[2].y, q[3].y});
    const double qb = std::min({q[0].y, q[1].y, q[2].y, q[3].y});

    const double qldx = q[0].x - q[1].x;
    const double qrdx = q[3].x - q[2].x;
    const double qldy = q[0].y - q[1].y;
    const double qrdy = q[3].y - q[2].y;

    const float* src = src_.ptr();
    float*       dst = fb.ptr();
    for (int32_t y = 0; y <= phi; ++y) {
      const double yfr = y*1. / phi;

      const double lxf = qldx * yfr + q[1].x;
      const double rxf = qrdx * yfr + q[2].x;
      const double ax = (rxf - lxf) / pwi;

      const double lyf = qldy * yfr + q[1].y;
      const double ryf = qrdy * yfr + q[2].y;
      const double ay  = (ryf - lyf) / pwi;

      for (int32_t x = 0; x <= pwi; ++x) {
        const double xf = lxf + ax*x;
        const double yf = lyf + ay*x;
        if (std::abs(xf) > 1 || std::abs(yf) > 1) continue;

        int32_t srcx = static_cast<int32_t>((xf+1)/2 * srcw);
        int32_t srcy = srch - 1 - static_cast<int32_t>((yf+1)/2 * srch);
        if (srcx >= srcw) srcx = srcw - 1;
        if (srcy >= srch) srcy = srch - 1;

        const int32_t dstx = pli + x;
        const int32_t dsty = (h-pui) + y;
        if (dstx < 0 || w <= dstx) continue;
        if (dsty < 0 || h <= dsty) continue;

        dst[dstx + w*dsty] = src[srcx + srcw*srcy];
      }
    }
  }

  void SetSource(Colorbuffer&& src) {
    src_ = std::move(src);
  }

 private:
  Colorbuffer src_;
};


}