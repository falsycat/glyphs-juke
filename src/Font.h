#pragma once

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

#include "thirdparty/stb_truetype.h"

#include "common.h"
#include "iAllocator.h"
#include "Rasterbuffer.h"

namespace gj {


 class Font {
 public:
  Font() = delete;
  Font(Font&&) = delete;
  Font(const Font&) = delete;

  Font& operator=(Font&&) = delete;
  Font& operator=(const Font&) = delete;

  Font(iAllocator* alloc, const std::string& path) : alloc_(alloc) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) {
      Abort("failed to open: "+path);
    }

    ifs.seekg(0, std::ios::end);
    const size_t size = ifs.tellg();
    ifs.seekg(0);

    buf_ = alloc_->MakeUniqArray<uint8_t>(size);
    ifs.read(reinterpret_cast<char*>(buf_.get()), size);

    const int offset = stbtt_GetFontOffsetForIndex(buf_.get(), 0);
    if (!stbtt_InitFont(&stb_, buf_.get(), offset)) {
      Abort("invalid font: "+path);
    }
  }

  Colorbuffer RenderGlyph(char16_t c, uint32_t fontsize = 32) {
    int w, h;

    const float s = stbtt_ScaleForPixelHeight(&stb_, fontsize*1.f);

    const uint8_t* src = stbtt_GetCodepointBitmap(&stb_, 0, s, c, &w, &h, 0, 0);

    Colorbuffer ret(alloc_, w, h);
    float* dst = ret.ptr();

    for (int y = 0; y < h; ++y) {
      for (int x = 0; x < w; ++x) {
        *dst = static_cast<float>(*src*1./UINT8_MAX);
        ++dst, ++src;
      }
    }
    return std::move(ret);
  }
  Colorbuffer RenderGlyphs(const std::wstring& str, uint32_t fontsize = 32) {
    const float s = stbtt_ScaleForPixelHeight(&stb_, fontsize * 1.f);

    int ascent;
    stbtt_GetFontVMetrics(&stb_, &ascent, 0, 0);

    const int baseline = static_cast<int>(ascent * s);

    /* calculate bitmap size */
    float h = 0, w = 2;
    for (auto c : str) {
      int advance, lsb;
      stbtt_GetCodepointHMetrics(&stb_, c, &advance, &lsb);

      int x0, y0, x1, y1;
      const float x_shift = w - (int) w;
      stbtt_GetCodepointBitmapBoxSubpixel(&stb_, c, s, s, x_shift, 0, &x0, &y0, &x1, &y1);

      const int ch = y1 - y0;
      if (h < ch) h = static_cast<float>(ch);

      w += advance * s;
    }

    Colorbuffer buf(alloc_, static_cast<uint32_t>(w), static_cast<uint32_t>(h));
    buf.Clear();

    float* dst = buf.ptr();

    float x = 2;
    for (auto c : str) {
      int advance, lsb;
      stbtt_GetCodepointHMetrics(&stb_, c, &advance, &lsb);

      int x0, y0, x1, y1;
      const float x_shift = x - (int)x;
      stbtt_GetCodepointBitmapBoxSubpixel(&stb_, c, s, s, x_shift, 0, &x0, &y0, &x1, &y1);

      uint8_t ptr[64][64] = {0};
      stbtt_MakeCodepointBitmapSubpixel(&stb_, &ptr[0][0], x1-x0, y1-y0, 64, s, s, x_shift, 0, c);

      const int l = static_cast<int>(x) + x0;
      const int r = static_cast<int>(x) + x1;
      const int u = baseline + y0;
      const int b = baseline + y1;
      for (int y = 0; y < b-u; ++y) {
        for (int x = 0; x < r-l; ++x) {
          dst[y*static_cast<int>(w) + l+x] = static_cast<float>(ptr[y][x]*1. / UINT8_MAX);
        }
      }
      x += advance * s;
    }
    return std::move(buf);
  }

 private:
  iAllocator* alloc_;

  stbtt_fontinfo stb_;

  UniqPtr<uint8_t> buf_;
};


}