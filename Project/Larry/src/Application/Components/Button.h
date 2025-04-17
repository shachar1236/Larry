#pragma once

#include "Math.h"

namespace Larry {
    struct Button {
        Math::Vec2 dimentions;
        Math::Vec4 color;

        Button() {
            dimentions = Math::Vec2(0.0f);
            color = Math::Vec4(1.0f);
        }
    };
}
