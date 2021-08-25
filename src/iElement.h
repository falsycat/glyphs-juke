#pragma once

#include "Frame.h"
#include "Period.h"


namespace gj {


class iElement {
 public:
  iElement(iElement&&) = default;
  iElement(const iElement&) = default;

  iElement& operator=(iElement&&) = default;
  iElement& operator=(const iElement&) = default;

  iElement() = default;
  virtual ~iElement() = default;

  virtual void Update(Frame& f) = 0;

  /* Interfaces had better not have a variable but this is for optimization. */
  const Period period;
};


}