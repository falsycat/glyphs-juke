#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "iConsole.h"


namespace gj {


class Win32Console : public iConsole {
 public:
  Win32Console() = delete;
  Win32Console(Win32Console&&) = delete;
  Win32Console(const Win32Console&) = delete;

  Win32Console& operator=(Win32Console&&) = delete;
  Win32Console& operator=(const Win32Console&) = delete;

  Win32Console(iAllocator* alloc, uint32_t w, uint32_t h) :
      w_(w), h_(h), shown_(false), alive_(true),
      th_([this]() { main(); }),
      cb_main_(alloc, w, h), cb_sub_(alloc, w, h),
      tb_main_(alloc, w, h), tb_sub_(alloc, w, h),
      chars_(std::make_unique<CHAR_INFO[]>(static_cast<uint64_t>(w)*h)),
      win_(GetConsoleWindow()) {
    _ASSERT(win_);

    screen_ = GetStdHandle(STD_OUTPUT_HANDLE);
    _ASSERT(screen_ != INVALID_HANDLE_VALUE);

    CONSOLE_SCREEN_BUFFER_INFOEX size;
    size.cbSize = sizeof(size);

    GetConsoleScreenBufferInfoEx(screen_, &size);
    COORD c;
    c.X = w_;
    c.Y = h_;
    size.dwSize = c;

    size.srWindow.Left   = 0;
    size.srWindow.Right  = w_ + 1;
    size.srWindow.Top    = 0;
    size.srWindow.Bottom = h_ + 1;
    SetConsoleScreenBufferInfoEx(screen_, &size);

    ShowWindow(win_, FALSE);
    SetWindowLong(win_, GWL_STYLE, GetWindowLong(win_, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));
  }
  ~Win32Console() {
    alive_.store(false);
    th_.join();
  }

  void Show() override {
    shown_.store(true);
  }
  void Hide() override {
    shown_.store(false);
  }

  Colorbuffer& TakeColorbuffer() override {
    return cb_sub_;
  }
  void SwapColorbuffer() override {
    std::lock_guard<std::mutex> _(mtx_);
    std::swap(cb_main_, cb_sub_);
  }

  Textbuffer& TakeTextbuffer() override {
    return tb_sub_;
  }
  void SwapTextbuffer() override {
    std::lock_guard<std::mutex> _(mtx_);
    std::swap(tb_main_, tb_sub_);
  }

  uint32_t width() const override {
    return w_;
  }
  uint32_t height() const override {
    return h_;
  }

 private:
  const uint32_t w_, h_;

  std::atomic_bool alive_;
  std::atomic_bool shown_;
  std::mutex       mtx_;
  std::thread      th_;

  Colorbuffer cb_main_;
  Colorbuffer cb_sub_;

  Textbuffer tb_main_;
  Textbuffer tb_sub_;

  std::unique_ptr<CHAR_INFO[]> chars_;

  HANDLE screen_ = INVALID_HANDLE_VALUE;
  HWND   win_    = nullptr;

  void main();
};


}