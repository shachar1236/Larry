#pragma once

#include "Log.h"
#include "Math/Math.h"

namespace Larry {
    struct Transform {
        Math::Vec3 translation;
        Math::Vec3 scale;
        Math::Vec3 rotation_axis;
        float rotation_size;

        Math::Vec3 _realTranslation; // the current translation plus parent realTranslation
        int _realTranslationTimestemp = -1; // the loop timestemp when the realTranslation was calculated

        Transform() {
            translation = Math::Vec3(0.0f);
            scale = Math::Vec3(1.0f);
            rotation_axis = Math::Vec3(0.0f, 0.0f, 1.0f);
            rotation_size = 0;
        }

        ~Transform() {

        }

        void Reset(int test) {
            LA_CORE_INFO("Reset debug test num1: ", test);
            translation = Math::Vec3(0.0f);
            scale = Math::Vec3(1.0f);
            rotation_axis = Math::Vec3(0.0f, 0.0f, 1.0f);
            rotation_size = 0;
        }

        YAML::Node EncodeYAML() {
            YAML::Node node;
            node["translation"] = translation;
            node["scale"] = scale;
            node["rotation_axis"] = rotation_axis;
            node["rotation_size"] = rotation_size;
            return node;
        }

        bool DecodeYAML(const YAML::Node& node) {
            translation = node["translation"].as<Math::Vec3>();
            scale = node["scale"].as<Math::Vec3>();
            rotation_axis = node["rotation_axis"].as<Math::Vec3>();
            rotation_size = node["rotation_size"].as<float>();
            return true;
        }
    };
}
