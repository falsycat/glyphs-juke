#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "iAllocator.h"

namespace gj {


template <typename T>
class Rasterbuffer {
 public:
  Rasterbuffer() = delete;
  Rasterbuffer(const Rasterbuffer&) = delete;
  Rasterbuffer& operator=(const Rasterbuffer&) = delete;

  Rasterbuffer(Rasterbuffer&& src) = default;
  Rasterbuffer& operator=(Rasterbuffer&& src) = default;

  Rasterbuffer(iAllocator* alloc, uint32_t w, uint32_t h) :
      alloc_(alloc), w_(w), h_(h),
      buf_(alloc->MakeUniqArray<T>(static_cast<uint64_t>(w_)*h_)) {
  }

  void Clear() {
    std::memset(buf_.get(), 0, static_cast<uint64_t>(w_) * h_ * sizeof(T));
  }

  uint32_t width() const {
    return w_;
  }
  uint32_t height() const {
    return h_;
  }
  T* ptr() {
    return buf_.get();
  }
  const T* ptr() const {
    return buf_.get();
  }

 private:
  iAllocator* alloc_;

  uint32_t w_, h_;
  UniqPtr<T> buf_;
};

using Colorbuffer = Rasterbuffer<float>;
using Textbuffer  = Rasterbuffer<char16_t>;


}