#include "LarryApiFunctions.h"
#include "Input.h"
#include "Log.h"
#include "LuaScripts.h"
#include "World.hpp"
#include "ECS/CPPApi/World.hpp"
#include "TextureLoader.h"
#include <functional>
#include <string>

using namespace Larry;

extern "C" {
    void* LuaLoadTexture(void* world_, const char* path, Larry::TextureConfig config) {
        ECS::Internal::World* iworld = (ECS::Internal::World*)world_;

        ECS::World world(iworld);

    TextureLoader* loader = world.GetSingelton<TextureLoader>();
        return loader->LoadTexture(path, config);
    }

    void LuaECSSystem(ECS_Entity entity, ECS_AnyQueue components, bool* stop) {
        Scripts::LuaScripts* lua_scripts = Scripts::LuaScripts::GetInstance();

        lua_scripts->LuaSystemCallback(entity, components, stop);
    }

    bool LuaKeyPressed(int key) {
        return Input::KeyPressed(key);
    }

    int LuaGetMouseX() {
        return Input::MouseX();
    }

    int LuaGetMouseY() {
        return Input::MouseY();
    }
}
