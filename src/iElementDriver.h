#pragma once

#include <map>
#include <string>
#include <variant>


namespace gj {


class iElementDriver {
 public:
  using Value = std::variant<int64_t, double, std::string>;
  using Param = std::map<std::string, Value>;

  iElementDriver(iElementDriver&&) = default;
  iElementDriver(const iElementDriver&) = default;

  iElementDriver& operator=(iElementDriver&&) = default;
  iElementDriver& operator=(const iElementDriver&) = default;

  iElementDriver() = default;
  virtual ~iElementDriver() = default;

  virtual void Update(Param&, double t) = 0;
};


}