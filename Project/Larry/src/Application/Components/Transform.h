#pragma once

#include "Math.h"

namespace Larry {
    struct Transform {
        Math::Vec3 translation;
        Math::Vec3 scale;
        Math::Vec3 rotation_axis;
        float rotation_size;

        Transform() {
            translation = Math::Vec3(0.0f);
            scale = Math::Vec3(1.0f);
            rotation_axis = Math::Vec3(0.0f, 0.0f, 1.0f);
            rotation_size = 0;
        }

    };
}
