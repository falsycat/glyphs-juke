#pragma once

#include <cmath>
#include <cstdint>
#include <string>

#include "Frame.h"
#include "iDrawable.h"
#include "iWritable.h"
#include "iScene.h"
#include "Logger.h"
#include "TickingClock.h"


namespace gj {


class Game : public iDrawable, public iWritable {
 public:
  static constexpr size_t kReserveDrawable = 256;
  static constexpr size_t kReserveWritable = 64;

  struct Param {
    iAllocator*   alloc;
    const iClock* clock;

    uint32_t w, h;
  };

  Game() = delete;
  Game(Game&&) = delete;
  Game(const Game&) = delete;

  Game& operator=(Game&&) = delete;
  Game& operator=(const Game&) = delete;

  Game(Param&& p);

  void Update(const std::string& input) {
    clock_.Tick();

    frame_.Clear();
    frame_.input = input;

    UniqPtr<iScene> next = scene_->Update(frame_);
    if (next) {
      scene_ = std::move(next);
    }
  }

  void Draw(Colorbuffer& fb) const override {
    frame_.Draw(fb);
  }
  void Write(Textbuffer& fb) const override {
    frame_.Write(fb);
    logger_.Write(fb);
  }

 private:
  iAllocator*  alloc_;
  TickingClock clock_;

  Logger logger_;

  uint32_t w_, h_;
  Frame    frame_;

  UniqPtr<iScene> scene_;
};


}