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

        YAML::Node EncodeYAML() {
            YAML::Node node;
            node["projection"] = projection;
            node["projection_layers"] = projection_layers;
            return node;
        }

        bool DecodeYAML(const YAML::Node& node) {
            projection = node["projection"].as<Math::Mat4>();
            projection_layers = node["projection_layers"].as<std::unordered_set<int>>();
            return true;
        }
    };
}
