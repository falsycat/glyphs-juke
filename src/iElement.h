#pragma once

#include "Frame.h"
#include "Period.h"


namespace gj {


class iElement {
 public:
  iElement() = delete;
  iElement(iElement&&) = default;
  iElement(const iElement&) = default;

  iElement& operator=(iElement&&) = default;
  iElement& operator=(const iElement&) = default;

  virtual ~iElement() = default;

  iElement(const Period& p) : period(p) {
  }

  virtual void Update(Frame& frame, double t) = 0;

  /* Interfaces had better not have a variable but this is for optimization. */
  const Period period;
};


}