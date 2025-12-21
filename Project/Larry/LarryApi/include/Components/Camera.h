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

        void SetPos(Math::Vec3 pos) {
            view = Math::lookAt(pos, Math::Vec3(pos.x, pos.y, 1.0f), Math::Vec3(0.0f, 1.0f, 0.0f));
        }

        YAML::Node EncodeYAML() {
            YAML::Node node;
            node["view"] = view;
            for (auto&& num : view_layers) {
                node["view_layers"].push_back(num);
            }
            return node;
        }

        bool DecodeYAML(const YAML::Node& node) {
            view = node["view"].as<Math::Mat4>();
            YAML::Node view_layers_node = node["view_layers"];
            for (int i = 0; i < view_layers_node.size(); i++) {
                view_layers.insert(view_layers_node[i].as<int>());
            }
            return true;
        }
    };
}
