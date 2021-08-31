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
  constexpr uint8_t with_text_attrs[] = {
    BACKGROUND_INTENSITY,
    FOREGROUND_INTENSITY,
    BACKGROUND_RED | FOREGROUND_INTENSITY,
    BACKGROUND_INTENSITY | FOREGROUND_RED,
  };

  constexpr size_t char_expr_count = sizeof(chars)/sizeof(chars[0])-1;
  constexpr size_t attr_expr_count = sizeof(attrs)/sizeof(attrs[0]);

  constexpr size_t reso = char_expr_count*attr_expr_count;

  /* posteffect */
  color = static_cast<float>(std::pow(color, 1.9));

  /* clamps alpha */
  int8_t a = static_cast<int8_t>(color*reso);
  if (a >= reso) a = reso-1;
  if (a <  0)    a = 0;

  /* calculates an index of attr from the alpha */
  size_t ci = a%char_expr_count;
  size_t ai = a/char_expr_count;
  if (ai%2 == 1) ci = char_expr_count-ci-1;

  /* assigns char and attr, but switches a set of attr depending on whether there's text */
  c.Char.UnicodeChar = chars[ci];
  if (text) {
    /*  if color of this pixel is expressed by a thick char (ci is bigger),
     * uses a attr set specialized in text to make it more visible. */
    c.Attributes       = ci < char_expr_count/3*2? attrs[ai]: with_text_attrs[ai];
    c.Char.UnicodeChar = text;
  } else {
    c.Attributes = attrs[ai];
  }
}

void gj::Win32Console::main() {
  bool shown = false;

  while (alive_.load()) {
    if (shown_.load()) {
      if (!shown) {
        shown = true;
        ShowWindow(win_, TRUE);
      }

      /* throws away the cursor */
      constexpr CONSOLE_CURSOR_INFO cursor{ 1, FALSE };
      SetConsoleCursorInfo(screen_, &cursor);

      /* read input */
      {  /* critical section */
        std::lock_guard<std::mutex> _(mtx_);

        INPUT_RECORD input[256];
        DWORD input_n;
        PeekConsoleInput(buffer_, input, 256, &input_n);
        if (input_n) {
          /*  Peek* can retrieve the input but doesn't remove it from the buffer
           * so calls Read* to clear. */
          ReadConsoleInput(buffer_, input, 256, &input_n);
        }
        for (DWORD i = 0; i < input_n; ++i) {
          const auto& rec = input[i];
          switch (rec.EventType) {
          case KEY_EVENT:
            if (rec.Event.KeyEvent.bKeyDown) {
              input_ += rec.Event.KeyEvent.uChar.AsciiChar;
            }
            break;
          default:
            break;
          }
        }
      }

      /* write output */
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
              mb = *tb > UINT8_MAX;
            }
            ++cb, ++tb, ++c;
          }
        }
      }
      
      /* writes calculated pixels into the front buffer */
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
    Sleep(10);
  }
}