#pragma once

#include "Math.h"
#include "TextureObject.h"

namespace Larry {
    struct Quad {
        Math::Vec3 dimentions;
        Math::Vec4 color;
        Ref<TextureObject> texture;

        Quad() {
            dimentions = Math::Vec3(0.0f);
            color = Math::Vec4(1.0f);
            texture = nullptr;
        }
    };
}
