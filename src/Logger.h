#pragma once

#include <vector>

#include "iDrawable.h"
#include "iLogger.h"
#include "Text.h"


namespace gj {


class Logger : public iWritable, public iLogger {
 public:
  Logger(Logger&&) = delete;
  Logger(const Logger&) = delete;

  Logger& operator=(Logger&&) = delete;
  Logger& operator=(const Logger&) = delete;

  Logger(uint32_t height) : height_(height) {
  }

  void Write(Textbuffer& fb) const override {
    for (const auto& text : lines_) {
      text.Write(fb);
    }
  }

  void Print(const std::wstring& msg) override {
    size_t n = lines_.size();

    if (n >= height_) {
      lines_.erase(lines_.begin());
      --n;
      for (size_t i = 0; i < n; ++i) {
        lines_[i].SetPosition(0, static_cast<int32_t>(i));
      }
    }

    Text text(msg, 0, static_cast<uint32_t>(n));
    lines_.push_back(std::move(text));
  }

 private:
  uint32_t height_;

  std::vector<Text> lines_;
};


}