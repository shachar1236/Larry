#pragma once

#include "TextureConfig.h"
#include "ECS_C.h"
#include "Math/Math.h"

extern "C" {
    void* LuaLoadTexture(void* world, const char* path, Larry::TextureConfig config);

    void LuaECSSystem(ECS_Entity entity, ECS_AnyQueue components, bool* stop);

    bool LuaKeyPressed(int key);
    int LuaGetMouseX();
    int LuaGetMouseY();
}
