#pragma once

#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
#include "ECS_C.h"
#include "LarryMemory.h"
#include "ECS/CPPApi/World.hpp"
#include <string>

namespace Larry::Scripts {
    class LuaScripts {
        private:
            lua_State *L;
            std::unordered_map<std::string, ECS_TypeHashCode> script_types;

            static LuaScripts* instance;

            LuaScripts(ECS::World*);
        public:
            static void InitLuaScripts(ECS::World* world) {
                instance = new LuaScripts(world);
            }

            static LuaScripts* GetInstance() {
                return LuaScripts::instance;
            }
            ~LuaScripts();

            lua_State* GetState() { return L; }
            std::unordered_map<std::string, ECS_TypeHashCode> GetScriptTypes() { return  script_types; }

            void AddScriptToEntity(const std::string& script_name, ECS::World* world, ECS_Entity entity);

            void UpdateScripts(ECS::Internal::World* world, double deltaTime);

            void test(ECS::Internal::World*);
            
            void RegisterComponent(const std::string& name, ECS_TypeHashCode hash_code);

            void LuaSystemCallback(ECS_Entity entity, ECS_AnyQueue components, bool* stop);
    };
}
