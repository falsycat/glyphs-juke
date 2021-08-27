#include "HiraganaMatcher.h"

#include <cassert>
#include <map>

#include "common.h"


static const size_t kPatternMax = 3;

static const std::map<std::wstring, std::vector<std::wstring>> kPatterns = {
  { L"あ", { L"a" } },
  { L"い", { L"i" } },
  { L"う", { L"u" } },
  { L"え", { L"e" } },
  { L"お", { L"o" } },
  { L"か", { L"ka" } },
  { L"き", { L"ki" } },
  { L"く", { L"ku" } },
  { L"け", { L"ke" } },
  { L"こ", { L"ko" } },
  { L"が", { L"ga" } },
  { L"ぎ", { L"gi" } },
  { L"ぐ", { L"gu" } },
  { L"げ", { L"ge" } },
  { L"ご", { L"go" } },
  { L"さ", { L"sa" } },
  { L"し", { L"si", L"shi" }},
  { L"す", { L"su" } },
  { L"せ", { L"se" } },
  { L"そ", { L"so" } },
  { L"ざ", { L"za" } },
  { L"じ", { L"zi", L"ji" }},
  { L"ず", { L"zu" } },
  { L"ぜ", { L"ze" } },
  { L"ぞ", { L"zo" } },
  { L"た", { L"ta" } },
  { L"ち", { L"ti", L"chi" }},
  { L"つ", { L"tu", L"tsu"}},
  { L"て", { L"te" } },
  { L"と", { L"to" } },
  { L"だ", { L"da" } },
  { L"ぢ", { L"di" } },
  { L"づ", { L"du" } },
  { L"で", { L"de" } },
  { L"ど", { L"do" } },
  { L"な", { L"na" } },
  { L"に", { L"ni" } },
  { L"ぬ", { L"nu" } },
  { L"ね", { L"ne" } },
  { L"の", { L"no" } },
  { L"は", { L"ha" } },
  { L"ひ", { L"hi" } },
  { L"ふ", { L"hu", L"fu" }},
  { L"へ", { L"he" } },
  { L"ほ", { L"ho" } },
  { L"ば", { L"ba" } },
  { L"び", { L"bi" } },
  { L"ぶ", { L"bu" } },
  { L"べ", { L"be" } },
  { L"ぼ", { L"bo" } },
  { L"ぱ", { L"pa" } },
  { L"ぴ", { L"pi" } },
  { L"ぷ", { L"pu" } },
  { L"ぺ", { L"pe" } },
  { L"ぽ", { L"po" } },
  { L"ま", { L"ma" } },
  { L"み", { L"mi" } },
  { L"む", { L"mu" } },
  { L"め", { L"me" } },
  { L"も", { L"mo" } },
  { L"や", { L"ya" } },
  { L"ゆ", { L"yu" } },
  { L"よ", { L"yo" } },
  { L"きゃ", { L"kya" } },
  { L"きゅ", { L"kyu" } },
  { L"きょ", { L"kyo" } },
  { L"しゃ", { L"sha", L"sya" }},
  { L"しゅ", { L"shu", L"syu" }},
  { L"しょ", { L"sho", L"syo" }},
  { L"じゃ", { L"ja", L"zya" }},
  { L"じゅ", { L"ju", L"zyu" }},
  { L"じぇ", { L"je", L"zye" }},
  { L"じょ", { L"jo", L"zyo" }},
  { L"ちゃ", { L"cha", L"tya" }},
  { L"ちゅ", { L"chu", L"tyu" }},
  { L"ちょ", { L"cho", L"tyo" }},
  { L"ら", { L"ra" } },
  { L"り", { L"ri" } },
  { L"る", { L"ru" } },
  { L"れ", { L"re" } },
  { L"ろ", { L"ro" } },
  { L"わ", { L"wa" } },
  { L"を", { L"wo" } },
  { L"ん", { L"n", L"nn"}},

  { L"っ", { L"ltu", L"xtu" }},
  { L"っか", { L"kka" } },
  { L"っき", { L"kki" } },
  { L"っく", { L"kku" } },
  { L"っけ", { L"kke" } },
  { L"っこ", { L"kko" } },
  { L"っが", { L"gga" } },
  { L"っぎ", { L"ggi" } },
  { L"っぐ", { L"ggu" } },
  { L"っげ", { L"gge" } },
  { L"っご", { L"ggo" } },
  { L"っさ", { L"ssa" } },
  { L"っし", { L"ssi" } },
  { L"っす", { L"ssu" } },
  { L"っせ", { L"sse" } },
  { L"っそ", { L"sso" } },
  { L"っざ", { L"zza" } },
  { L"っじ", { L"zzi", L"jji" }},
  { L"っず", { L"zzu" } },
  { L"っぜ", { L"zze" } },
  { L"っぞ", { L"zzo" } },
  { L"った", { L"tta" } },
  { L"っち", { L"tti", L"cchi" }},
  { L"っつ", { L"ttu", L"ttsu"}},
  { L"って", { L"tte" } },
  { L"っと", { L"tto" } },
  { L"っだ", { L"dda" } },
  { L"っぢ", { L"ddi" } },
  { L"っづ", { L"ddu" } },
  { L"っで", { L"dde" } },
  { L"っど", { L"ddo" } },
  { L"っな", { L"nna" } },
  { L"っに", { L"nni" } },
  { L"っぬ", { L"nnu" } },
  { L"っね", { L"nne" } },
  { L"っの", { L"nno" } },
  { L"っは", { L"hha" } },
  { L"っひ", { L"hhi" } },
  { L"っふ", { L"hhu", L"ffu" }},
  { L"っへ", { L"hhe" } },
  { L"っほ", { L"hho" } },
  { L"っば", { L"bba" } },
  { L"っび", { L"bbi" } },
  { L"っぶ", { L"bbu" } },
  { L"っべ", { L"bbe" } },
  { L"っぼ", { L"bbo" } },
  { L"っぱ", { L"ppa" } },
  { L"っぴ", { L"ppi" } },
  { L"っぷ", { L"ppu" } },
  { L"っぺ", { L"ppe" } },
  { L"っぽ", { L"ppo" } },
  { L"っま", { L"mma" } },
  { L"っみ", { L"mmi" } },
  { L"っむ", { L"mmu" } },
  { L"っめ", { L"mme" } },
  { L"っも", { L"mmo" } },
  { L"っや", { L"yya" } },
  { L"っゆ", { L"yyu" } },
  { L"っよ", { L"yyo" } },
  { L"っら", { L"rra" } },
  { L"っり", { L"rri" } },
  { L"っる", { L"rru" } },
  { L"っれ", { L"rre" } },
  { L"っろ", { L"rro" } },
  { L"わ", { L"wwa" } },
  { L"を", { L"wwo" } },
};


bool gj::HiraganaMatcher::Input_(wchar_t c, bool force_cut) {
  assert(!done());

  const std::wstring newbuf = force_cut? buffer_: buffer_ + c;
  const std::wstring remain = state_.text.substr(state_.match);

  const size_t pattern_len = std::min(kPatternMax, remain.size());

  bool   accept     = false;
  size_t part_match = 0;
  size_t comp_match = 0;
  for (size_t i = 1; i <= pattern_len; ++i) {
    const auto& p = kPatterns.find(remain.substr(0, i));
    if (p == kPatterns.end()) continue;

    const auto& preds = p->second;
    for (const auto& itr : preds) {
      if (newbuf.size() == 0 || itr.size() < newbuf.size()) {
        continue;
      }

      if (itr.substr(0, newbuf.size()) == newbuf) {
        accept = true;
        if (itr.size() == newbuf.size()) {
          comp_match = i;
          if (force_cut) {
            buffer_ = L"";
            state_.match += i;
            return Input_(c, false);
          }
        } else {
          part_match = i;
        }
      }
    }
  }

  if (force_cut) {
    return false;
  }
  if (!accept) {
    return Input_(c, true);
  }

  if (comp_match > part_match) {
    buffer_ = L"";
    state_.match += comp_match;
    UpdateExpects_();
    return true;
  }

  buffer_ += c;
  UpdateExpects_();
  return true;
}

void gj::HiraganaMatcher::UpdateExpects_() {
  std::wstring remain = state_.text.substr(state_.match);

  expects_ = L"";

  bool first = true;
  while (remain.size()) {
    const size_t prev = remain.size();
    for (size_t len = std::min(kPatternMax, remain.size()); len > 0; --len) {
      const auto& p = kPatterns.find(remain.substr(0, len));
      if (p == kPatterns.end()) continue;

      const auto& preds = p->second;
      if (first) {
        for (const auto& itr : preds) {
          if (itr.size() < buffer_.size()) continue;
          if (itr.substr(0, buffer_.size()) == buffer_) {
            expects_ += itr.substr(buffer_.size());
            first = false;
            break;
          }
        }
        if (first) continue;
      } else {
        expects_ += preds[0];
      }
      remain = remain.substr(len);
    }
    if (prev == remain.size()) {
      Abort("invalid pattern for InputWin");
    }
  }
}