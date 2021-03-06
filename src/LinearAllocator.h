#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#include "common.h"
#include "iAllocator.h"


namespace gj {


class LinearAllocator : public iAllocator {
public:
  using iAllocator::Alloc;
  using iAllocator::MakeUniq;

  LinearAllocator() = delete;
  LinearAllocator(LinearAllocator&&) = delete;
  LinearAllocator(const LinearAllocator&) = delete;

  LinearAllocator& operator=(LinearAllocator&&) = delete;
  LinearAllocator& operator=(const LinearAllocator&) = delete;

  LinearAllocator(void* ptr, const size_t size) :
      ptr_(static_cast<uint8_t*>(ptr)) {
    assert(size >= sizeof(Header)*2);

    auto head = reinterpret_cast<Header*>(ptr_);
    head->prev = 0;
    head->next = size - sizeof(Header);
    head->size = sizeof(Header);

    auto tail = reinterpret_cast<Header*>(ptr_+head->next);
    tail->prev = head->next;
    tail->next = 0;
    tail->size = sizeof(Header);
  }

  void* Alloc(const size_t size) override {
    const size_t aligned_size = (((size >> 3) + !!(size & 0b111))) << 3;
    const size_t whole_size = sizeof(Header) + aligned_size;
    assert(aligned_size >= size);

    auto h = reinterpret_cast<Header*>(ptr_);

    /* finds block that has enough unused space at the tail */
    while (h->next) {
      const size_t remain = h->next - h->size;
      if (remain >= whole_size) {
        auto hprev = h;
        auto hnext = reinterpret_cast<Header*>(reinterpret_cast<uint8_t*>(hprev) + hprev->next);

        /* creates new block */
        h = reinterpret_cast<Header*>(reinterpret_cast<uint8_t*>(hprev) + hprev->size);
        h->prev = hprev->size;
        h->next = remain;
        h->size = whole_size;

        /* links the created block with prev and next */
        hprev->next = h->prev;
        hnext->prev = h->next;

        return h + 1;
      }
      h = reinterpret_cast<Header*>(reinterpret_cast<uint8_t*>(h) + h->next);
    }
    Abort("LinearAllocator Allocation Failure");
  }

  void Free(void* ptr) override {
    if (!ptr) return;
    uint8_t* uptr = reinterpret_cast<uint8_t*>(ptr) - sizeof(Header);

    auto h = reinterpret_cast<Header*>(uptr);
    auto hprev = reinterpret_cast<Header*>(uptr - h->prev);
    auto hnext = reinterpret_cast<Header*>(uptr + h->next);

    /* unlinks and drops the block */
    hprev->next += h->next;
    hnext->prev += h->prev;
  }

 private:
  /*         |------prev--------||--------next--------|        */
  /*                             |----size-----|               */
  /* memory: HHAAAAAAAAAAAAAAAAUUHHAAAAAAAAAAAAAUUUUUUUHHAAAAA */
  /*         H: header, A: allocated area, U: unused area      */
  struct Header {
    size_t  prev;
    size_t  next;
    size_t  size;
  };
  static_assert(sizeof(Header) == sizeof(size_t)*3, "padding detected in memory header");

  uint8_t* const ptr_;
};


}