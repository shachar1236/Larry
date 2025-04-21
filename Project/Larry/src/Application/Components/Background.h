#pragma once

#include "Math/Math.h"
#include "TextureLoader/TextureObject.h"

namespace Larry {
    struct Background {
        Math::Vec4 color;
        Ref<TextureObject> texture;

        Background() {
            color = Math::Vec4(1.0f);
            texture = nullptr;
        }
    };
}
