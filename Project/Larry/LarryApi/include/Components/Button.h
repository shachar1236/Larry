#pragma once

#include "Math/Math.h"

namespace Larry {
    struct Button {
        Math::Vec2 dimentions;
        Math::Vec4 color;

        Button() {
            dimentions = Math::Vec2(0.0f);
            color = Math::Vec4(1.0f);
        }

        YAML::Node EncodeYAML() {
            YAML::Node node;
            node["dimentions"] = dimentions;
            node["color"] = color;
            return node;
        }

        bool DecodeYAML(const YAML::Node& node) {
            dimentions = node["dimentions"].as<Math::Vec2>();
            color = node["color"].as<Math::Vec4>();
            return true;
        }
    };
}
