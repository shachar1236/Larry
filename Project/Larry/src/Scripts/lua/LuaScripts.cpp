#include "LuaScripts.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "ECS_C.h"
#include "LarryApiFunctions.h"
#include "Log.h"
#include "TextureLoader.h"
#include "World.hpp"
#include "lua.h"
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>

namespace Larry::Scripts {

    LuaScripts* LuaScripts::instance;

    LuaScripts::LuaScripts(const Ref<ECS::World>&) {
        L = luaL_newstate();
        if (!L) {
            LA_CORE_ERROR("Could not create new lua state");
            return;
        }
        luaL_openlibs(L); /* Open standard libraries */

        if (luaL_dofile(L, "LarryApi.lua") != LUA_OK) {
            LA_CORE_ERROR("Errot loading lua script: {}", lua_tostring(L, -1));
            return;
        }
    }

    LuaScripts::~LuaScripts() {
        lua_settop(L, 0); /* (4) */
        lua_close(L);
    }

    void LuaScripts::test(ECS::Internal::World* iworld) {
        lua_getglobal(L, "Test");
        const int EntityNum = 3;

        lua_pushlightuserdata(L, iworld);

        lua_pushlightuserdata(L, reinterpret_cast<void*>(static_cast<uintptr_t>(EntityNum)));

        if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
            LA_CORE_ERROR("Cant call function from lua!!!");
            return;
        }
    }

    void LuaScripts::RegisterComponent(const std::string& name, ECS_TypeHashCode hash_code) {
        lua_getglobal(L, "AddComponentHash");

        lua_pushstring(L, name.c_str());
        lua_pushlightuserdata(L, reinterpret_cast<void*>(static_cast<uintptr_t>(hash_code)));

        if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
            LA_CORE_ERROR("Cant register component {} in lua!", name);
            return;
        }
    }

    void LuaScripts::LuaSystemCallback(ECS_Entity entity, ECS_AnyQueue components, bool* stop) {
        LA_CORE_TRACE("In LuaSystemCallback");
        lua_getglobal(L, "SystemCallback");

        lua_pushlightuserdata(L, reinterpret_cast<void*>(static_cast<uintptr_t>(entity)));
        lua_pushlightuserdata(L, components);
        lua_pushlightuserdata(L, stop);

        if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
            LA_CORE_ERROR("Cant call system callback in lua!");
            return;
        }
    }
}
