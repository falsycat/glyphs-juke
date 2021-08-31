#include "Lua.h"

#include "thirdparty/lualib.h"


/* pushes variant to Lua stack */
struct LuaPusher {
  LuaPusher() = delete;
  LuaPusher(lua_State* L) : L(L) {
  }

  void operator()(int64_t v) {
    lua_pushinteger(L, v);
  }
  void operator()(double v) {
    lua_pushnumber(L, v);
  }
  void operator()(const std::string& v) {
    lua_pushstring(L, v.c_str());
  }

 private:
  lua_State* L;
};

/* takes a value in Lua stack to variant */
struct LuaTaker {
  LuaTaker() = delete;
  LuaTaker(lua_State* L, int index) : L(L), index_(index) {
  }

  void operator()(int64_t& v) {
    v = luaL_checkinteger(L, index_);
  }
  void operator()(double& v) {
    v = luaL_checknumber(L, index_);
  }
  void operator()(std::string& v) {
    v = luaL_checkstring(L, index_);
  }

 private:
  lua_State* L;
  int index_;
};


class LuaFunc : public gj::iElementDriver {
 public:
  LuaFunc() = delete;
  LuaFunc(LuaFunc&&) = delete;
  LuaFunc(const LuaFunc&) = delete;

  LuaFunc& operator=(LuaFunc&&) = delete;
  LuaFunc& operator=(const LuaFunc&) = delete;

  LuaFunc(lua_State* L, int index) : L(L) {
    /* registers function and param table*/
    lua_pushvalue(L, index);
    func_ = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_createtable(L, 0, 0);
    table_ = luaL_ref(L, LUA_REGISTRYINDEX);
  }
  ~LuaFunc() {
    luaL_unref(L, LUA_REGISTRYINDEX, func_);
  }

  void Update(Param& param, double t) override {
    /* pushes registered func */
    lua_rawgeti(L, LUA_REGISTRYINDEX, func_);

    /* pushes current time as the first argument */
    lua_pushnumber(L, t);
    
    /* pushes param table as the second argument */
    lua_rawgeti(L, LUA_REGISTRYINDEX, table_);
    for (const auto& p : param) {
      lua_pushstring(L, p.first.c_str());
      std::visit(LuaPusher(L), p.second);
      lua_rawset(L, -3);
    }

    /* calls the function */
    const int ret = lua_pcall(L, 2, 0, 0);
    if (ret) {
      gj::Abort(std::string("Lua error: ")+lua_tostring(L, -1));
    }

    /* copies values from Lua stack into the param map */
    lua_rawgeti(L, LUA_REGISTRYINDEX, table_);
    for (auto& p : param) {
      lua_pushstring(L, p.first.c_str());
      lua_rawget(L, -2);
      std::visit(LuaTaker(L, -1), p.second);
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
  gj::ElementStore* store =
    reinterpret_cast<gj::ElementStore*>(lua_touserdata(L, lua_upvalueindex(2)));
  gj::iElementFactory* factory =
    reinterpret_cast<gj::iElementFactory*>(lua_touserdata(L, lua_upvalueindex(3)));

  const int n = lua_gettop(L);

  /* takes and validates arguments */
  const lua_Integer st = luaL_checkinteger(L, 1);
  const lua_Integer ed = luaL_checkinteger(L, 2);
  if (st >= ed) {
    return luaL_error(L, "invalid period");
  }
  if (!lua_isfunction(L, n)) {
    return luaL_error(L, "no driver specified");
  }

  gj::iElementFactory::Param param;
  param.period = gj::Period(st, ed);
  param.driver = alloc->MakeUniq<LuaFunc>(L, n);

  /* takes custom params from Lua stack */
  for (int i = 3; i < n; ++i) {
    gj::iElementFactory::Param::CustomValue v;
    if (lua_isnumber(L, i)) {
      v = lua_tonumber(L, i);
    } else if (lua_isstring(L, i)) {
      v = lua_tostring(L, i);
    } else {
      return luaL_error(L, "invalid args");
    }
    param.custom.push_back(v);
  }

  auto e = factory->Create(std::move(param));
  if (!e) return luaL_error(L, "factory returned nullptr");
  store->Schedule(std::move(e));
  return 0;
}


gj::Lua::Lua(iAllocator* alloc, ElementStore* store, const FactoryMap& factory, const std::string& path) {
  L = luaL_newstate();
  if (L == nullptr) {
    Abort("lua_newstate failure");
  }
  luaopen_math(L);

  /* registers all factories as Lua function */
  for (const auto& f : factory) {
    lua_pushstring(L, f.first.c_str());

    lua_pushlightuserdata(L, alloc);
    lua_pushlightuserdata(L, store);
    lua_pushlightuserdata(L, f.second);
    lua_pushcclosure(L, CallFactory_, 3);

    lua_rawset(L, LUA_GLOBALSINDEX);
  }

  /* executes the Lua script */
  if (0 != luaL_loadfile(L, path.c_str())) {
    const char* msg = lua_tostring(L, -1);
    Abort(std::string("luaL_loadfile failure: ") + msg);
  }
  const int ret = lua_pcall(L, 0, 0, 0);
  if (ret) {
    gj::Abort(std::string("Lua error: ") + lua_tostring(L, -1));
  }
}