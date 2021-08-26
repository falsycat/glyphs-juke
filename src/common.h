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


[[noreturn]]
static inline void Abort(const std::string& msg) {
  MessageBox(NULL, ConvertStrToWstr(msg).c_str(), L"PROGRAM ABORTED", MB_OK);
  std::exit(1);
}


}