#pragma once

#include <cstdint>

#include "Rasterbuffer.h"


namespace gj {


class iWritable {
 public:
  iWritable(iWritable&&) = default;
  iWritable(const iWritable&) = default;

  iWritable& operator=(iWritable&&) = default;
  iWritable& operator=(const iWritable&) = default;

  iWritable() = default;
  virtual ~iWritable() = default;

  virtual void Write(Textbuffer&) const = 0;
};

class WritableBase : public iWritable {
 public:
  WritableBase(WritableBase&&) = default;
  WritableBase(const WritableBase&) = default;

  WritableBase& operator=(WritableBase&&) = default;
  WritableBase& operator=(const WritableBase&) = default;

  WritableBase(int32_t x, int32_t y) : x_(x), y_(y) {
  }

  void SetPosition(int32_t x, int32_t y) {
    x_ = x;
    y_ = y;
  }

 protected:
  int32_t x_ = 0, y_ = 0;
};


}