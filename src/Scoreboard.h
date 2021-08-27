#pragma once

#include <cstdint>


namespace gj {


struct Scoreboard {
 public:
  std::wstring title;

  size_t input   = 0;
  size_t correct = 0;
  size_t miss    = 0;

  size_t lines         = 0;
  size_t completeLines = 0;
};


}