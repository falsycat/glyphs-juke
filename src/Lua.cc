#include "Lua.h"


class LuaFunc : public gj::iElementDriver {
 public:
  LuaFunc() = delete;
  LuaFunc(LuaFunc&&) = delete;
  LuaFunc(const LuaFunc&) = delete;

  LuaFunc& operator=(LuaFunc&&) = delete;
  LuaFunc& operator=(const LuaFunc&) = delete;

  LuaFunc(lua_State* L) : L(L) {
    func_ = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_createtable(L, 0, 0);
    table_ = luaL_ref(L, LUA_REGISTRYINDEX);
  }
  ~LuaFunc() {
    luaL_unref(L, LUA_REGISTRYINDEX, func_);
  }

  void Update(Param& param) override {
    lua_rawgeti(L, LUA_REGISTRYINDEX, func_);

    lua_rawgeti(L, LUA_REGISTRYINDEX, table_);
    for (const auto& p : param) {
      lua_pushstring(L, p.first.c_str());
      if (std::holds_alternative<int64_t>(p.second)) {
        lua_pushinteger(L, std::get<int64_t>(p.second));
      } else if (std::holds_alternative<double>(p.second)) {
        lua_pushnumber(L, std::get<double>(p.second));
      } else if (std::holds_alternative<std::string>(p.second)) {
        const std::string str = std::get<std::string>(p.second);
        lua_pushstring(L, str.c_str());
      } else {
        assert(false);
      }
      lua_rawset(L, -3);
    }

    const int ret = lua_pcall(L, 1, 0, 0);
    if (ret) {
      gj::Abort(std::string("Lua error: ")+lua_tostring(L, -1));
    }

    lua_rawgeti(L, LUA_REGISTRYINDEX, table_);
    for (auto& p : param) {
      lua_pushstring(L, p.first.c_str());
      lua_rawget(L, -2);

      if (std::holds_alternative<int64_t>(p.second)) {
        p.second = luaL_checkinteger(L, -1);
      } else if (std::holds_alternative<double>(p.second)) {
        p.second = luaL_checknumber(L, -1);
      } else if (std::holds_alternative<std::string>(p.second)) {
        p.second = luaL_checkstring(L, -1);
      } else {
        assert(false);
      }
      lua_pop(L, 1);
    }
    lua_pop(L, 1);
  }

 private:
  lua_State* L;

  int func_  = LUA_REFNIL;
  int table_ = LUA_REFNIL;
};


static int CallFactory_(lua_State* L) {
  gj::iAllocator* alloc =
    reinterpret_cast<gj::iAllocator*>(lua_touserdata(L, lua_upvalueindex(1)));
  gj::iElementFactory* factory =
    reinterpret_cast<gj::iElementFactory*>(lua_touserdata(L, lua_upvalueindex(2)));

  const lua_Integer st = luaL_checkinteger(L, 1);
  const lua_Integer ed = luaL_checkinteger(L, 2);
  if (st >= ed) {
    return luaL_error(L, "invalid period");
  }
  if (!lua_isfunction(L, 3)) {
    return luaL_error(L, "no driver specified");
  }

  lua_pushvalue(L, 3);
  factory->Create(gj::Period(st, ed), alloc->MakeUniq<gj::iElementDriver, LuaFunc>(L));
  return 0;
}


gj::Lua::Lua(iAllocator* alloc, const FactoryMap& factory, const std::string& path) {
  L = luaL_newstate();
  if (L == nullptr) {
    Abort("lua_newstate failure");
  }

  for (const auto& f : factory) {
    lua_pushstring(L, f.first.c_str());

    lua_pushlightuserdata(L, alloc);
    lua_pushlightuserdata(L, f.second);
    lua_pushcclosure(L, CallFactory_, 2);

    lua_rawset(L, LUA_GLOBALSINDEX);
  }

  if (0 != luaL_loadfile(L, path.c_str())) {
    const char* msg = lua_tostring(L, -1);
    Abort(std::string("luaL_loadfile failure: ") + msg);
  }
  const int ret = lua_pcall(L, 0, 0, 0);
  if (ret) {
    gj::Abort(std::string("Lua error: ") + lua_tostring(L, -1));
  }
}