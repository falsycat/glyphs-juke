#pragma once

#include <string>


namespace gj {


class iInputMatcher {
 public:
  struct State {
    std::wstring text;
    size_t       match;
  };

  iInputMatcher(iInputMatcher&&) = default;
  iInputMatcher(const iInputMatcher&) = default;

  iInputMatcher& operator=(iInputMatcher&&) = default;
  iInputMatcher& operator=(const iInputMatcher&) = default;

  iInputMatcher() = default;
  virtual ~iInputMatcher() = default;

  virtual bool Input(wchar_t c) = 0;

  virtual const std::wstring& expects() const = 0;

  virtual bool done() const = 0;

  virtual const State& state() const = 0;
};


}