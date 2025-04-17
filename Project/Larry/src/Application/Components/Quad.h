#pragma once

#include "Math.h"

namespace Larry {
    struct Quad {
        Math::Vec3 dimentions;
        Math::Vec4 color;

        Quad() {
            dimentions = Math::Vec3(0.0f);
            color = Math::Vec4(1.0f);
        }
    };
}
