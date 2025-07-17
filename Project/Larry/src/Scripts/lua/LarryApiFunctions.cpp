#include "LarryApiFunctions.h"
#include "Log.h"
#include "LuaScripts.h"
#include "World.hpp"
#include "ECS/CPPApi/World.hpp"
#include "TextureLoader.h"

using namespace Larry;

extern "C" {
    void* LuaLoadTexture(void* world_, const char* path, Larry::TextureConfig config) {
        ECS::Internal::World* iworld = (ECS::Internal::World*)world_;

        ECS::World world(iworld);

    TextureLoader* loader = world.GetSingelton<TextureLoader>();
        return loader->LoadTexture(path, config);
    }

    void LuaECSSystem(ECS_Entity entity, ECS_AnyQueue components, bool* stop) {
        LA_CORE_TRACE("In LuaECSSystem");
        Scripts::LuaScripts* lua_scripts = Scripts::LuaScripts::GetInstance();

        lua_scripts->LuaSystemCallback(entity, components, stop);
    }
}
