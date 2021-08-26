#define STB_TRUETYPE_IMPLEMENTATION
#include "Font.h"

gj::Colorbuffer gj::Font::RenderGlyphs(const std::wstring& str, uint32_t fontsize) {
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
    const float x_shift = w - (int)w;
    stbtt_GetCodepointBitmapBoxSubpixel(&stb_, c, s, s, x_shift, 0, &x0, &y0, &x1, &y1);

    const int ch = baseline + y1;
    if (h < ch) h = static_cast<float>(ch);

    w += advance * s;
  }
  w += 1;
  h += 1;

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
    if (x1 - x0 > 64 || y1 - y0 > 64) {
      gj::Abort("font size is too huge");
    }

    uint8_t ptr[64][64] = { 0 };
    stbtt_MakeCodepointBitmapSubpixel(&stb_, &ptr[0][0], x1 - x0, y1 - y0, 64, s, s, x_shift, 0, c);

    const int l = static_cast<int>(x) + x0;
    const int r = static_cast<int>(x) + x1;
    const int u = baseline + y0;
    const int b = baseline + y1;
    for (int y = 0; y < b - u; ++y) {
      for (int x = 0; x < r - l; ++x) {
        dst[(u + y) * static_cast<int>(w) + l + x] = static_cast<float>(ptr[y][x] * 1. / UINT8_MAX);
      }
    }
    x += advance * s;
  }
  return std::move(buf);
}