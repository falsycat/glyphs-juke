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

  ElementStore(ElementStore&&) = default;
  ElementStore(const ElementStore&) = delete;

  ElementStore& operator=(ElementStore&&) = default;
  ElementStore& operator=(const ElementStore&) = delete;

  ElementStore(size_t reserve) {
    pending_.reserve(reserve);
    performing_.reserve(reserve);
  }

  void Schedule(UniqPtr<iElement>&& e) {
    const uint64_t st = e->period.start;

    /* finds an iterator of an element
     * that will perform right after the element which is trying to add */
    auto insert_pos = std::find_if(pending_.begin(), pending_.end(),
      [st](auto& x) { return x->period.start > st; });

    /* inserts the new element right before the found element,
     * this means pending_ stays sorted by time that each element's performance will start */
    pending_.insert(insert_pos, std::move(e));
  }

  void Update(Frame& frame, uint64_t now) {
    /* finds elements whose period is started and
     * move these elements to performing_ from pending_.
     * such elements are always on head of pending_ because it's sorted */
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

    /* updates elements currently performing and deletes expired ones by assigning nullptr */
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

  bool IsEmpty() const {
    return pending_.empty() && performing_.empty();
  }

  size_t CountPreparings() const {
    size_t n = 0;
    for (const auto& e : pending_) {
      if (!e->HasPrepared()) ++n;
    }
    return n;
  }
  size_t size() const {
    return pending_.size() + performing_.size();
  }

 private:
  std::vector<UniqPtr<iElement>> pending_;
  std::vector<UniqPtr<iElement>> performing_;
};


}