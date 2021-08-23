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

    buf_ = alloc_->MakeUniqueArray<uint8_t>(size);
    ifs.read(reinterpret_cast<char*>(buf_.get()), size);

    const int offset = stbtt_GetFontOffsetForIndex(buf_.get(), 0);
    if (!stbtt_InitFont(&stb_, buf_.get(), offset)) {
      Abort("invalid font: "+path);
    }
  }

  Colorbuffer&& RenderGlyph(char16_t c, uint32_t fontsize = 32) {
    int w, h;

    const float s = stbtt_ScaleForPixelHeight(&stb_, fontsize*1.f);

    const uint8_t* src = stbtt_GetCodepointBitmap(&stb_, 0, s, c, &w, &h, 0, 0);

    Colorbuffer ret(alloc_, w, h);
    float* dst = ret.ptr();

    for (int y = 0; y < h; ++y) {
      for (int x = 0; x < w; ++x) {
        *dst = *src*1./UINT8_MAX;
        ++dst, ++src;
      }
    }
    return std::move(ret);
  }

 private:
  iAllocator* alloc_;

  stbtt_fontinfo stb_;

  iAllocator::UniquePtr<uint8_t> buf_;
};


}