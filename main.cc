#include <iostream>
#include <string>

#include <windows.h>

#include "common.h"
#include "Font.h"
#include "LinearAllocator.h"
#include "Win32Console.h"


constexpr size_t kHeapSize = 1024*1024*64;

constexpr uint32_t kWidth  = 96;
constexpr uint32_t kHeight = 28;

int main() {
  auto memory = std::make_unique<uint8_t[]>(kHeapSize);
  gj::LinearAllocator alloc(memory.get(), kHeapSize);

  gj::Win32Console console(&alloc, kWidth, kHeight);
  console.Show();

  gj::Font font(&alloc, "./font/shippori.ttf");
  gj::Colorbuffer glyph = font.RenderGlyph(L'繍', 50);

  size_t t = 0;
  while (true) {
    {
      const std::u16string s = u"世界は猫によって支配されている";

      gj::Textbuffer& tb = console.TakeTextbuffer();
      tb.Clear();

      char16_t* ptr = tb.ptr() + kWidth*(kHeight/2)+(t%(kWidth-s.length()*2));
      for (size_t i = 0; i < s.length(); ++i) {
        ptr[i*2] = s[i];
      }
      console.SwapTextbuffer();
    }

    {
      gj::Colorbuffer& cb = console.TakeColorbuffer();
      float* ptr = cb.ptr();

      constexpr uintmax_t max = kHeight + kWidth;
      for (uint32_t y = 0; y < kHeight; ++y) {
        for (uint32_t x = 0; x < kWidth; ++x) {
          *ptr = static_cast<float>((static_cast<uintmax_t>(y)+x+t)%(max*2)*1./max);
          if (*ptr > 1) *ptr = 2-*ptr;
          ++ptr;
        }
      }

      ptr = cb.ptr();
      const uint32_t w = glyph.width();
      const uint32_t h = glyph.height();
      const float* src = glyph.ptr();
      for (uint32_t y = 0; y < h; ++y) {
        if (y >= kHeight) break;
        for (uint32_t x = 0; x < w; ++x) {
          if (x*2+1 >= kWidth) break;
          ptr[y*kWidth+x*2]   += src[y*w+x]*.3;
          ptr[y*kWidth+x*2+1] += src[y*w+x]*.3;
        }
      }

      console.SwapColorbuffer();
    }

    Sleep(60);
    ++t;
  }
}