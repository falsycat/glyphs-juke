#pragma once

#include "iAllocator.h"
#include "iElement.h"
#include "iElementDriver.h"
#include "Period.h"


namespace gj {


class iElementFactory {
 public:
  iElementFactory(iElementFactory&&) = default;
  iElementFactory(const iElementFactory&) = default;

  iElementFactory& operator=(iElementFactory&&) = default;
  iElementFactory& operator=(const iElementFactory&) = default;

  iElementFactory() = default;
  virtual ~iElementFactory() = default;

  virtual UniqPtr<iElement> Create(const Period& p, UniqPtr<iElementDriver>&& drv) = 0;
};


}