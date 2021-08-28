#pragma once

#include <cassert>
#include <cstddef>
#include <memory>


namespace gj {


class iAllocator {
public:
  template <typename T>
  struct Deleter {
    Deleter() = default;
    Deleter(Deleter&&) = default;
    Deleter(const Deleter&) = delete;

    Deleter& operator=(Deleter&&) = default;
    Deleter& operator=(const Deleter&) = delete;

    template <typename I>
    Deleter(Deleter<I>&& src) {
      alloc_ = src.alloc_;
    }
    explicit Deleter(iAllocator* alloc) : alloc_(alloc) {
    }

    void operator()(T* ptr) {
      assert(alloc_);
      ptr->~T();
      alloc_->Free(ptr);
    }

    iAllocator* alloc_;
  };

  template <typename T>
  using UniqPtr = std::unique_ptr<T, iAllocator::Deleter<T>>;

  iAllocator(iAllocator&&) = default;
  iAllocator(const iAllocator&) = default;

  iAllocator& operator=(iAllocator&&) = default;
  iAllocator& operator=(const iAllocator&) = default;

  iAllocator() = default;

  virtual ~iAllocator() = default;

  virtual void* Alloc(const size_t size) = 0;
  virtual void Free(void* ptr) = 0;

  template <typename T>
  T* Alloc(size_t n = 1) {
    return reinterpret_cast<T*>(Alloc(sizeof(T) * n));
  }

  template <typename T, typename... Args>
  UniqPtr<T> MakeUniq(Args&&... args) {
    T* ptr = Alloc<T>();
    return std::unique_ptr<T, Deleter<T>>(new(ptr) T(std::forward<Args>(args)...), Deleter<T>(this));
  }
  template <typename T>
  UniqPtr<T> MakeUniqArray(size_t n) {
    T* ptr = Alloc<T>(n);
    return std::unique_ptr<T, Deleter<T>>(ptr, Deleter<T>(this));
  }
};

template <typename T>
using UniqPtr = iAllocator::UniqPtr<T>;


}