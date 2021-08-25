#pragma once

#include <string>


namespace gj {


class iLogger {
 public:
  iLogger(iLogger&&) = delete;
  iLogger(const iLogger&) = delete;

  iLogger& operator=(iLogger&&) = delete;
  iLogger& operator=(const iLogger&) = delete;

  iLogger() = default;
  virtual ~iLogger() = default;

  virtual void Print(const std::wstring& msg) = 0;
};


}