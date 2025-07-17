#pragma once

#include "TextureConfig.h"
#include "ECS_C.h"

extern "C" {
    void* LuaLoadTexture(void* world, const char* path, Larry::TextureConfig config);

    void LuaECSSystem(ECS_Entity entity, ECS_AnyQueue components, bool* stop);
}
