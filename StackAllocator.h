#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#include "iAllocator.h"


namespace gj {


class StackAllocator : public iAllocator {
public:
  using iAllocator::Alloc;
  using iAllocator::MakeUnique;

  StackAllocator() = delete;
  StackAllocator(StackAllocator&&) = delete;
  StackAllocator(const StackAllocator&) = delete;

  StackAllocator& operator=(StackAllocator&&) = delete;
  StackAllocator& operator=(const StackAllocator&) = delete;

  StackAllocator(void* ptr, const size_t size) :
      begin_(static_cast<uint8_t*>(ptr)),
      end_  (begin_+size),
      next_ (begin_) {
  }
  ~StackAllocator() {
    assert(refcnt_ == 0);
  }

  void* Alloc(const size_t size) override {
    const size_t aligned_size = ((size >> 3) + !!(size & 0b111)) << 3;

    void* ret = next_;
    next_ += aligned_size;
    if (next_ >= end_) {
      return nullptr;
    }
    ++refcnt_;
    return ret;
  }
  void Free(void* ptr) override {
    if (!ptr) return;

    assert(refcnt_);
    if (--refcnt_ == 0) {
      next_ = begin_;
    }
  }

private:
  uint8_t* const begin_;
  uint8_t* const end_;
  uint8_t* next_;

  size_t refcnt_ = 0;
};


}