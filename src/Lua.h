#pragma once

#include <map>
#include <string>

#include "thirdparty/lua.hpp"

#include "common.h"
#include "iAllocator.h"
#include "iElementFactory.h"


namespace gj {


class Lua {
 public:
  using FactoryMap = std::map<std::string, iElementFactory*>;

  Lua() = delete;
  Lua(Lua&&) = delete;
  Lua(const Lua&) = delete;

  Lua& operator=(Lua&&) = delete;
  Lua& operator=(const Lua&) = delete;

  Lua(iAllocator* alloc, const FactoryMap& factory, const std::string& path);

  ~Lua() {
    lua_close(L);
  }

 private:
  lua_State* L = nullptr;
};


}