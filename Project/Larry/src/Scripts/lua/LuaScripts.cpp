#include "LuaScripts.h"
#include "Components/Transform.h"
#include "ECS_C.h"
#include "Log.h"
#include "World.hpp"
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>

namespace Larry::Scripts {
    void LuaScript::Init(ECS::Internal::World* world, ECS_TypeHashCode code) {
        lua_State *L = luaL_newstate();
        if (!L) {
            LA_CORE_ERROR("Could not create new lua state");
            return;
        }
        luaL_openlibs(L); /* Open standard libraries */
        if (luaL_dofile(L, "ECS.lua") != LUA_OK) {
            LA_CORE_ERROR("Errot loading lua script: {}", lua_tostring(L, -1));
            return;
        }
        lua_getglobal(L, "GetComponent");

        lua_pushlightuserdata(L, world);

        // 2. ECS_Entity (int64_t)
        // Lua numbers can represent 64-bit integers directly in LuaJIT
        // lua_pushinteger(L, 1);
        lua_pushlightuserdata(L, reinterpret_cast<void*>(static_cast<uintptr_t>(3)));

        // 3. ECS_TypeHashCode (unsigned long)
        // Lua numbers can represent unsigned long
        // lua_pushinteger(L, code);
        lua_pushlightuserdata(L, reinterpret_cast<void*>(static_cast<uintptr_t>(code)));

        if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
            LA_CORE_ERROR("Cant call function from lua!!!");
            lua_close(L);
            return;
        }

        lua_settop(L, 0); /* (4) */
        lua_close(L);
    }
}
