#include "HiraganaMatcher.h"

#include <cassert>
#include <map>

#include "common.h"


static const std::vector<std::pair<std::wstring, std::wstring>> kPatterns = {
  { L"ka", L"か", },
  { L"ki", L"き", },
  { L"ku", L"く", },
  { L"ke", L"け", },
  { L"ko", L"こ", },
  { L"sa", L"さ", },
  { L"si", L"し", },
  { L"su", L"す", },
  { L"se", L"せ", },
  { L"so", L"そ", },
  { L"ta", L"た", },
  { L"ti", L"ち", },
  { L"tu", L"つ", },
  { L"te", L"て", },
  { L"to", L"と", },
  { L"na", L"な", },
  { L"ni", L"に", },
  { L"nu", L"ぬ", },
  { L"ne", L"ね", },
  { L"no", L"の", },
  { L"a", L"あ", },
  { L"i", L"い", },
  { L"u", L"う", },
  { L"e", L"え", },
  { L"o", L"お", },
};


bool gj::HiraganaMatcher::Input(wchar_t c) {
  assert(!done());

  const std::wstring remain = state_.text.substr(state_.match);
  const std::wstring newbuf = buffer_ + c;

  const size_t n = newbuf.size();

  size_t last_match = 0;
  bool   accept     = false;
  for (size_t i = 0; i < kPatterns.size(); ++i) {
    const auto& pattern = kPatterns[i];
    if (pattern.first.substr(0, n) == newbuf) {
      const size_t pn = pattern.second.size();
      if (remain.substr(0, pn) == pattern.second) {
        accept     = true;
        last_match = i;
      }
      if (pattern.first.size() == n) {
        if (!accept) return false;
        state_.match += pn;
        buffer_ = L"";
        UpdateExpects(i);
        return true;
      }
    }
  }

  if (accept) {
    buffer_ = newbuf;
    UpdateExpects(last_match);
  }
  return accept;
}

void gj::HiraganaMatcher::UpdateExpects(size_t last_match) {
  std::wstring text = state_.text.substr(state_.match);

  expects_ = L"";
  if (text.empty()) return;

  if (last_match < SIZE_MAX && buffer_.size()) {
    expects_ = kPatterns[last_match].first.substr(buffer_.size());
    text     = text.substr(kPatterns[last_match].second.size());
  }

  while (text.size()) {
    bool match = false;
    for (const auto& pattern : kPatterns) {
      const size_t n = pattern.second.size();
      if (text.size() < n) continue;

      if (text.substr(0, n) == pattern.second) {
        expects_ += pattern.first;
        text      = text.substr(n);
        match     = true;
        break;
      }
    }
    if (!match) Abort("hiragana janai");
  }
}