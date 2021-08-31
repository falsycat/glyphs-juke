#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "thirdparty/linalg.h"
#include "thirdparty/utf8.h"

namespace gj {


using mat3 = ::linalg::mat<double, 3, 3>;
using vec3 = ::linalg::vec<double, 3>;

constexpr double kPi = 3.14159265358979323846264338327950288;


static inline std::wstring ConvertStrToWstr(const std::string& str) {
  std::wstring ret;

  const void* c = str.c_str();
  for (;;) {
    int32_t code;
    c = utf8codepoint(c, &code);
    if (!code) return ret;
    ret += static_cast<wchar_t>(code);
  }
}

static inline size_t CountWstrBytes(const std::wstring& str) {
  size_t n = 0;
  for (const auto c : str) {
    n += c > UINT8_MAX? 2: 1;
  }
  return n;
}

static inline uint64_t XorShift(uint64_t x) {
  x = x ^ (x << 13);
  x = x ^ (x >>  7);
  return x ^ (x << 17);
}


[[noreturn]]
static inline void Abort(const std::wstring& msg) {
  MessageBox(NULL, msg.c_str(), L"PROGRAM ABORTED", MB_ICONWARNING);
  std::exit(1);
}
[[noreturn]]
static inline void Abort(const std::string& msg) {
  Abort(ConvertStrToWstr(msg));
}



}