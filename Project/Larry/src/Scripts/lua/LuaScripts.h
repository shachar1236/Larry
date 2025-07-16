#pragma once

#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
#include "ECS_C.h"
#include "LarryMemory.h"
#include "World.hpp"
#include <string>

namespace Larry::Scripts {
    class LuaScripts {
        private:
            lua_State *L;
        public:
            LuaScripts(const Ref<ECS::World>&);
            ~LuaScripts();

            void test(ECS::Internal::World*);
            
            void RegisterComponent(const std::string& name, ECS_TypeHashCode hash_code);
    };
}
