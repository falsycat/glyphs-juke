#pragma once

#include <codecvt>
#include <cstdlib>
#include <sstream>
#include <string>

#include <windows.h>

namespace gj {


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