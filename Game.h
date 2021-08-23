#pragma once

#include <cmath>
#include <string>

#include "iDrawable.h"
#include "iWritable.h"
#include "Logger.h"
#include "Texture.h"


namespace gj {


class Game : public iDrawable, public iWritable {
 public:
  Game() = delete;
  Game(Game&&) = delete;
  Game(const Game&) = delete;

  Game& operator=(Game&&) = delete;
  Game& operator=(const Game&) = delete;

  Game(iAllocator* alloc, uint32_t w, uint32_t h) :
      alloc_(alloc), w_(w), h_(h), logger_(h), test_(Colorbuffer(alloc, 1, 1)) {
    Colorbuffer temp(alloc_, 5, 5);
    float* ptr = temp.ptr();
    for (size_t i = 0; i < 25; ++i) ptr[i] = i%2*.3+.7;
    test_ = Texture(std::move(temp));
  }

  void Update() {
    static int i = 0;
    ++i;
    if (i%100 == 0) logger_.Print(L"すべての人類は死滅する: "+std::to_wstring(i));

    double t = i%200/200.;
    t = 1 - t;

    mat3 m = mat3{ {.2, 0, 0},{0, .2, 0},{0, 0, 1} };

    const double s = sin(3.14*2*t*t*t), c = cos(3.14*2*t*t*t);
    m = ::linalg::mul(mat3{{c, -s, 0,}, {s, c, 0}, {0, 0, 1}}, m);
    m = ::linalg::mul(mat3{{ 1, 0, 0,}, {0, 1, 0}, {(1 - t * t * t * 2) * .6, 0, 1}}, m);
    m = ::linalg::mul(mat3{ {1, 0, 0},{0, 16/9., 0},{0, 0, 1} }, m);
    test_.SetMatrix(m);
  }

  void Draw(Colorbuffer& fb) const override {
    test_.Draw(fb);
  }
  void Write(Textbuffer& fb) const override {
    logger_.Write(fb);
  }

 private:
  iAllocator* alloc_;

  uint32_t w_, h_;
  Logger logger_;

  Texture test_;
};


}