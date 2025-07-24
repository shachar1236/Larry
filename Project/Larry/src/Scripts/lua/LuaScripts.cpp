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
            LA_CORE_ERROR("Error loading lua script: {}", lua_tostring(L, -1));
            return;
        }
    }

    LuaScripts::~LuaScripts() {
        lua_settop(L, 0); /* (4) */
        lua_close(L);
    }

    void LuaScripts::AddScriptToEntity(const std::string& script_name, const Ref<ECS::World>& world, ECS_Entity entity) {
        ECS::Internal::World* iworld = world->GetInternalWorld();
        ECS_TypeHashCode script_hash = std::hash<std::string>()(script_name);
        script_types[script_name] = script_hash;

        ECS_RegisterType(iworld, script_hash, sizeof(int), [](const void* x){ 
            int r = *static_cast<const int*>(x);
            lua_State* L = LuaScripts::GetInstance()->GetState();

            lua_getglobal(L, "CallScriptOnDelete");
            lua_rawgeti(L, LUA_REGISTRYINDEX, r);
            if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
                LA_CORE_WARN("Cant call lua script OnDelete, error: {}", lua_tostring(L, -1));
            }

            luaL_unref(L, LUA_REGISTRYINDEX, r);
        });

        if (luaL_dofile(L, script_name.c_str()) != LUA_OK) {
            LA_CORE_ERROR("Error loading lua script: {}", lua_tostring(L, -1));
            return;
        }

        int r = luaL_ref(L, LUA_REGISTRYINDEX);
        // adding to ECS
        ECS::Internal::AnyQueue* resultQueue = iworld->InitAnyQueue();
        ECS::Internal::TypeQueue* types = iworld->InitTypeQueue();

        types->push_back(script_hash);
        bool success = iworld->InsertComponents(entity, *types, *resultQueue);

        if (!success) {
            luaL_unref(L, LUA_REGISTRYINDEX, r);

            iworld->DoneWithAnyQueue(resultQueue);
            iworld->DoneWithTypeQueue(types);
            return;
        }

        resultQueue->InitPopBack();
        int* p = (int*)resultQueue->PopBack().value;
        *p = r;

        iworld->DoneWithAnyQueue(resultQueue);
        iworld->DoneWithTypeQueue(types);
        // call script on create
        lua_getglobal(L, "CallScriptOnCreate");

        lua_rawgeti(L, LUA_REGISTRYINDEX, r);
        lua_pushlightuserdata(L, iworld);
        lua_pushlightuserdata(L, reinterpret_cast<void*>(static_cast<uintptr_t>(entity)));

        if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
            LA_CORE_WARN("Cant call {}::OnCreate, error: {}", script_name, lua_tostring(L, -1));
            return;
        }
    }

    void LuaScripts::UpdateScripts(ECS::Internal::World* world, double deltaTime) {
        for (auto&& s : script_types) {

            ECS::Internal::AnyQueue* system_components_queue = world->InitAnyQueue();
            ECS::Internal::TypeQueue* types = world->InitTypeQueue();

            types->push_back(s.second);

            world->System(*types, *system_components_queue,
                [this, deltaTime](ECS_Entity entity, ECS::Internal::AnyQueue& components, bool* stop) {
                    components.InitPopBack();
                    int r = *(int*)components.PopBack().value;

                    lua_getglobal(L, "CallScriptOnUpdate");

                    lua_rawgeti(L, LUA_REGISTRYINDEX, r);
                    lua_pushnumber(L, deltaTime);

                    if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
                        LA_CORE_WARN("Cant call lua script OnUpdate, error: {}", lua_tostring(L, -1));
                        return;
                    }
                });
        }
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
