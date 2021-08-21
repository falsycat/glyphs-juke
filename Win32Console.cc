#include "Win32Console.h"

#include <cmath>


static void CalcChar(CHAR_INFO& c, float color, uint16_t text) {
  constexpr wchar_t chars[] = L" .,:x!|X#%@$M";
  constexpr uint8_t attrs[] = {
    FOREGROUND_INTENSITY,
    BACKGROUND_INTENSITY,
    BACKGROUND_INTENSITY | FOREGROUND_RED,
    BACKGROUND_RED | FOREGROUND_INTENSITY,
  };

  constexpr size_t char_expr_count = sizeof(chars)/sizeof(chars[0])-1;
  constexpr size_t attr_expr_count = sizeof(attrs)/sizeof(attrs[0]);

  constexpr size_t reso = char_expr_count*attr_expr_count;

  /* post effect */
  color = std::pow(color, 1.9);

  int8_t a = static_cast<int8_t>(color*reso);
  if (a >= reso) a = reso-1;
  if (a <  0)    a = 0;

  size_t ci = a%char_expr_count;
  size_t ai = a/char_expr_count;
  if (ai%2 == 1) ci = char_expr_count-ci-1;

  c.Char.UnicodeChar = chars[ci];
  c.Attributes       = attrs[ai];

  if (text) c.Char.UnicodeChar = text;
}

void gj::Win32Console::main() {
  bool shown = false;

  while (alive_.load()) {
    if (shown_.load()) {
      if (!shown) {
        shown = true;
        ShowWindow(win_, TRUE);
      }

      constexpr CONSOLE_CURSOR_INFO cursor{ 1, FALSE };
      SetConsoleCursorInfo(screen_, &cursor);

      CHAR_INFO* c = chars_.get();
      {  /* critical section */
        std::lock_guard<std::mutex> _(mtx_);

        const float*    cb = cb_main_.ptr();
        const char16_t* tb = tb_main_.ptr();

        for (uint32_t y = 0; y < h_; ++y) {
          bool mb = false;
          for (uint32_t x = 0; x < w_; ++x) {
            if (mb) {
              *c = *(c - 1);
              (c - 1)->Attributes |= COMMON_LVB_LEADING_BYTE;
              c->Attributes |= COMMON_LVB_TRAILING_BYTE;
              mb = false;
            } else {
              CalcChar(*c, *cb, *tb);
              mb = (*tb & 0x80);
            }
            ++cb, ++tb, ++c;
          }
        }
      }

      const COORD size = { static_cast<SHORT>(w_), static_cast<SHORT>(h_), };
      const COORD pos  = { 0, 0, };
      SMALL_RECT  rc   = { 0, 0, static_cast<SHORT>(w_), static_cast<SHORT>(h_), };
      WriteConsoleOutput(screen_, chars_.get(), size, pos, &rc);

    } else {
      if (shown) {
        ShowWindow(win_, FALSE);
        shown = false;
      }
    }
    Sleep(30);
  }
}