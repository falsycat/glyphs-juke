#include <iostream>
#include <string>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "common.h"
#include "Font.h"
#include "Game.h"
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

  gj::Game game(&alloc, kWidth, kHeight);
  while (true) {
    game.Update();
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