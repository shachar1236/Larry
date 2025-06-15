#include "LuaScripts.h"
#include "Log.h"
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>

namespace Larry::Scripts {
    void LuaScript::Init() {
        lua_State *L = luaL_newstate();
        if (!L) {
            LA_CORE_ERROR("Could not create new lua state");
            return;
        }
        luaL_openlibs(L); /* Open standard libraries */
        /* Load config file */
            // luaL_loadfile(L, argv[1]); /* (1) */
        luaL_dostring(L, "print \"This is printed from lua!!!!\"");
        int ret = lua_pcall(L, 0, 0, 0);
        lua_settop(L, 0); /* (4) */
        lua_close(L);
    }
}
