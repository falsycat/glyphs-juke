#include <iostream>
#include <string>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "common.h"
#include "Font.h"
#include "Game.h"
#include "LinearAllocator.h"
#include "SystemClock.h"
#include "Win32Console.h"


constexpr size_t kHeapSize = 1024*1024*64;

constexpr uint32_t kWidth  = 96;
constexpr uint32_t kHeight = 28;

int main() {
  auto memory = std::make_unique<uint8_t[]>(kHeapSize);
  gj::LinearAllocator alloc(memory.get(), kHeapSize);

  gj::Win32Console console(&alloc, kWidth, kHeight);
  console.Show();

  gj::Game::Param param;
  param.alloc = &alloc;
  param.clock = &gj::SystemClock::instance();
  param.w     = kWidth;
  param.h     = kHeight;
  gj::Game game(std::move(param));
  while (true) {
    game.Update(console.TakeInput());
    {
      auto& fb = console.TakeColorbuffer();
      fb.Clear();
      game.Draw(fb);
      console.SwapColorbuffer();
    }
    {
      auto& fb = console.TakeTextbuffer();
      fb.Clear();
      game.Write(fb);
      console.SwapTextbuffer();
    }
    Sleep(10);
  }
}