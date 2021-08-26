#pragma once

#include <algorithm>
#include <vector>

#include "iAllocator.h"
#include "iClock.h"
#include "iElement.h"


namespace gj {


class ElementStore {
 public:
  ElementStore() = delete;

  ElementStore(ElementStore&&) = delete;
  ElementStore(const ElementStore&) = delete;

  ElementStore& operator=(ElementStore&&) = delete;
  ElementStore& operator=(const ElementStore&) = delete;

  ElementStore(iClock* clock, size_t reserve) : clock_(clock) {
    pending_.reserve(reserve);
    performing_.reserve(reserve);
  }

  void Schedule(UniqPtr<iElement>&& e) {
    const uint64_t st = e->period.start;

    auto insert_pos = std::find_if(pending_.begin(), pending_.end(),
      [st](auto& x) { return x->period.start > st; });

    pending_.insert(insert_pos, std::move(e));
  }

  void Update(Frame& frame) {
    const uint64_t now = clock_->now();

    auto pending_beg = pending_.begin();
    auto pending_end = pending_.end();
    auto pending_itr = pending_beg;
    for (; pending_itr < pending_end; ++pending_itr) {
      iElement* e = pending_itr->get();
      if (e->period.start > now) {
        break;
      }
      performing_.push_back(std::move(*pending_itr));
    }
    pending_.erase(pending_beg, pending_itr);

    for (auto& eptr : performing_) {
      iElement* e = eptr.get();
      if (e->period.end <= now) {
        e->Finalize();
        eptr = nullptr;
      } else {
        e->Update(frame, e->period.Normalize(now));
      }
    }

    /* erase nullptr */
    performing_.erase(
      std::remove_if(
        performing_.begin(), performing_.end(), [](auto& x) {return !x; }),
      performing_.end());
  }

  bool IsEmpty() {
    return pending_.empty() && performing_.empty();
  }

 private:
  const iClock* clock_;

  std::vector<UniqPtr<iElement>> pending_;
  std::vector<UniqPtr<iElement>> performing_;
};


}