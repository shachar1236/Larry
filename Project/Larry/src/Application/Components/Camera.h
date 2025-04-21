#pragma once

#include "Math/Math.h"
#include <unordered_set>

namespace Larry {
    struct Camera {
        Math::Mat4 view;
        std::unordered_set<int> view_layers; // the layers where this projection will be used, stored with layer ids

        Camera() {
            view_layers = std::unordered_set<int>();
            view = Math::lookAt(Math::Vec3(0.0f, 0.0f, 3.0f), Math::Vec3(0.0f), Math::Vec3(0.0f, 1.0f, 0.0f));
        }

        void SetPos(const Math::Vec3& pos) {
            view = Math::lookAt(pos, Math::Vec3(pos.x, pos.y, 1.0f), Math::Vec3(0.0f, 1.0f, 0.0f));
        }

    };
}
