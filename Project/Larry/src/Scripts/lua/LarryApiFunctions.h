#pragma once

#include "TextureConfig.h"

extern "C" {
    void* LuaLoadTexture(void* world, const char* path, Larry::TextureConfig config);
}
