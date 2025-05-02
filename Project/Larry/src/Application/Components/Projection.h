#pragma once

#include "Math/Math.h"
#include <unordered_set>

namespace Larry {
    struct Projection {
        Math::Mat4 projection;
        std::unordered_set<int> projection_layers; // the layers where this projection will be used, stored with layer ids

        Projection() {
            projection = Math::Mat4(1.0f);
            projection_layers = std::unordered_set<int>();
        }

    };
}
