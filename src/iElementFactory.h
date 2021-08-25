#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "iAllocator.h"
#include "iElement.h"
#include "iElementDriver.h"
#include "Period.h"


namespace gj {


class iElementFactory {
 public:
  struct Param {
    using CustomValue = std::variant<int64_t, double, std::string>;

    Period                   period;
    std::vector<CustomValue> custom;
    UniqPtr<iElementDriver>  driver;
  };

  iElementFactory(iElementFactory&&) = default;
  iElementFactory(const iElementFactory&) = default;

  iElementFactory& operator=(iElementFactory&&) = default;
  iElementFactory& operator=(const iElementFactory&) = default;

  iElementFactory() = default;
  virtual ~iElementFactory() = default;

  virtual UniqPtr<iElement> Create(Param&& p) = 0;
};


}