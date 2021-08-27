#pragma once

#include <cstdint>
#include <string>

#include "common.h"
#include "iWritable.h"

namespace gj {


class Text : public WritableBase {
 public:
  Text(Text&&) = default;
  Text(const Text&) = default;

  Text& operator=(Text&&) = default;
  Text& operator=(const Text&) = default;

  Text(const std::wstring& str, uint32_t x = 0, uint32_t y = 0) :
      WritableBase(x, y), entity_(str),
      w_(static_cast<uint32_t>(CountWstrBytes(str))) {
  }

  void Write(Textbuffer& fb) const override {
    const int32_t w = static_cast<int32_t>(fb.width());
    const int32_t h = static_cast<int32_t>(fb.height());

    char16_t* ptr = fb.ptr();

    int32_t x = x_, y = y_;
    if (y >= h) return;

    const size_t len = entity_.size();
    for (size_t i = 0; i < len; ++i) {
      if (x <  0) continue;
      if (x >= w) return;

      ptr[x+y*w] = entity_[i];
      x += (entity_[i] > UINT8_MAX)? 2: 1;
    }
  }

  const std::wstring& entity() const {
    return entity_;
  }
  uint32_t width() const {
    return w_;
  }

 private:
  std::wstring entity_;
  uint32_t w_;
};


}