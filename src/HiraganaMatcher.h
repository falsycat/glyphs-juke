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
    UpdateExpects_();
  }

  bool Input(wchar_t c) override {
    return Input_(c, false);
  }

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

  bool Input_(wchar_t c, bool force_cut);
  void UpdateExpects_();
};


}