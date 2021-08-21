#pragma once

#include <cstddef>
#include <cstdint>

#include "iAllocator.h"

namespace gj {


template <typename T>
class Rasterbuffer {
 public:
  Rasterbuffer() = delete;
  Rasterbuffer(const Rasterbuffer&) = delete;
  Rasterbuffer& operator=(const Rasterbuffer&) = delete;

  Rasterbuffer(iAllocator* alloc, uint32_t w, uint32_t h) :
      alloc_(alloc), w_(w), h_(h),
      buf_(alloc->Alloc<T>(static_cast<uint64_t>(w_)*h_)) {
    _ASSERT(buf_ != nullptr);
  }
  ~Rasterbuffer() {
    alloc_->Free(buf_);
  }

  Rasterbuffer(Rasterbuffer&& src) noexcept :
      alloc_(src.alloc_), w_(src.w_), h_(src.h_), buf_(src.buf_) {
    src.buf_ = nullptr;
  }
  Rasterbuffer& operator=(Rasterbuffer&& src) noexcept {
    if (this != &src) {
      alloc_ = src.alloc_;
      w_     = src.w_;
      h_     = src.h_;
      buf_   = src.buf_;
      src.buf_ = nullptr;
    }
    return *this;
  }


  void Clear() {
    memset(buf_, 0, static_cast<uint64_t>(w_) * h_ * sizeof(T));
  }


  uint32_t width() const {
    return w_;
  }
  uint32_t height() const {
    return h_;
  }
  T* ptr() {
    return buf_;
  }
  const T* ptr() const {
    return buf_;
  }

 private:
  iAllocator* alloc_;

  uint32_t w_, h_;
  T* buf_;
};

using Colorbuffer = Rasterbuffer<float>;
using Textbuffer  = Rasterbuffer<char16_t>;


}