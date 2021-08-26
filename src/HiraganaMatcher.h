#pragma once

#include <vector>

#include "iInputMatcher.h"

namespace gj {


class HiraganaMatcher : public iInputMatcher {
 public:
  HiraganaMatcher(HiraganaMatcher&&) = default;
  HiraganaMatcher(const HiraganaMatcher&) = default;

  HiraganaMatcher& operator=(HiraganaMatcher&&) = default;
  HiraganaMatcher& operator=(const HiraganaMatcher&) = default;

  HiraganaMatcher(const std::wstring& text) : state_{text, 0} {
    UpdateExpects();
  }

  bool Input(wchar_t c) override;

  const std::wstring& expects() const override {
    return expects_;
  }
  bool done() const override {
    return state_.text.size() <= state_.match;
  }
  const State& state() const override {
    return state_;
  }

 private:
  std::wstring buffer_;
  std::wstring expects_;
  
  State state_;

  void UpdateExpects(size_t last_match = SIZE_MAX);
};


}