#pragma once

#include <codecvt>
#include <cstdlib>
#include <sstream>
#include <string>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "thirdparty/linalg.h"

namespace gj {


using mat3 = ::linalg::mat<double, 3, 3>;
using vec3 = ::linalg::vec<double, 3>;


static inline std::wstring ConvertUtf8ToUtf16(const std::string& str) {
  std::wostringstream conv;
  conv << str.c_str();
  return conv.str();
}


[[noreturn]]
static inline void Abort(const std::string& msg) {
  MessageBox(NULL, ConvertUtf8ToUtf16(msg).c_str(), L"PROGRAM ABORTED", MB_OK);
  std::exit(1);
}


}