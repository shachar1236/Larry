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
            for (auto&& num : projection_layers) {
                node["projection_layers"].push_back(num);
            }
            return node;
        }

        bool DecodeYAML(const YAML::Node& node) {
            projection = node["projection"].as<Math::Mat4>();
            YAML::Node proj_layers_node = node["projection_layers"];
            for (int i = 0; i < proj_layers_node.size(); i++) {
                projection_layers.insert(proj_layers_node[i].as<int>());
            }
            return true;
        }
    };
}
