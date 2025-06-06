#pragma once

#include "Math/Math.h"
#include "TextureLoader/TextureObject.h"

namespace Larry {
    struct Quad {
        Math::Vec3 dimentions;
        Math::Vec4 color;
        Ref<TextureObject> texture = nullptr;

        Quad() {
            dimentions = Math::Vec3(0.0f);
            color = Math::Vec4(1.0f);
            texture = nullptr;
        }
    };
}
