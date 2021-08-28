#pragma once

#include <map>
#include <string>

#include "thirdparty/lua.hpp"

#include "common.h"
#include "iAllocator.h"
#include "iElementFactory.h"
#include "ElementStore.h"


namespace gj {


class Lua {
 public:
  using FactoryMap = std::map<std::string, iElementFactory*>;

  Lua() = delete;
  Lua(const Lua&) = delete;

  Lua& operator=(const Lua&) = delete;
  
  Lua(Lua&& src) noexcept : L(src.L) {
    src.L = nullptr;
  }
  Lua& operator=(Lua&& src) noexcept {
    if (&src != this) {
      L = src.L;
      src.L = nullptr;
    }
    return *this;
  }

  Lua(iAllocator* alloc, ElementStore* store, const FactoryMap& factory, const std::string& path);

  ~Lua() {
    if (L) lua_close(L);
  }

 private:
  lua_State* L = nullptr;
};


}