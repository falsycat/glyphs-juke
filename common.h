#pragma once

#include <codecvt>
#include <cstdlib>
#include <sstream>
#include <string>

#include <windows.h>

namespace gj {


[[noreturn]]
static inline void Abort(const std::string& msg) {
  std::wostringstream conv;
  conv << msg.c_str();

  MessageBox(NULL, conv.str().c_str(), L"PROGRAM ABORTED", MB_OK);
  std::exit(1);
}


}