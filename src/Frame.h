#pragma once

#include <string>
#include <vector>

#include "iDrawable.h"
#include "iWritable.h"


namespace gj {


class Frame : public iDrawable, public iWritable {
 public:
  Frame() = delete;
  Frame(Frame&&) = delete;
  Frame(const Frame&) = delete;

  Frame& operator=(Frame&&) = delete;
  Frame& operator=(const Frame&) = delete;

  Frame(uint32_t w, uint32_t h, size_t dres, size_t wres) : w(w), h(h) {
    draw_.reserve(dres);
    write_.reserve(wres);
  }

  void Clear() {
    draw_.clear();
    write_.clear();
  }

  void Add(const iDrawable* d) {
    draw_.push_back(d);
  }
  void Add(const iWritable* w) {
    write_.push_back(w);
  }

  void Draw(Colorbuffer& fb) const override {
    for (auto d : draw_) {
      d->Draw(fb);
    }
  }
  void Write(Textbuffer& fb) const override {
    for (auto w : write_) {
      w->Write(fb);
    }
  }

  std::string input;

  uint32_t w, h;

 private:
  std::vector<const iDrawable*> draw_;
  std::vector<const iWritable*> write_;
};


}