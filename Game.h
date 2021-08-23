#pragma once

#include <string>

#include "iDrawable.h"
#include "iWritable.h"
#include "Logger.h"


namespace gj {


class Game : public iDrawable, public iWritable {
 public:
  Game() = delete;
  Game(Game&&) = delete;
  Game(const Game&) = delete;

  Game& operator=(Game&&) = delete;
  Game& operator=(const Game&) = delete;

  Game(uint32_t w, uint32_t h) : w_(w), h_(h), logger_(h) {
    logger_.Print(L"こんにちは");
  }

  void Update() {
    static int i = 0;
    ++i;
    logger_.Print(L"すべての人類は死滅する: "+std::to_wstring(i));
  }

  void Draw(Colorbuffer& fb) const override {
  }
  void Write(Textbuffer& fb) const override {
    logger_.Write(fb);
  }

 private:
  uint32_t w_, h_;
  Logger logger_;
};


}