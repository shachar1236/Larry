#pragma once

#include "Utils/Log.h"
#include "Math/Math.h"
#include "TextureLoader/TextureLoader.h"
#include "TextureLoader/TextureObject.h"

namespace Larry {
    struct Quad {
        Math::Vec3 dimentions;
        Math::Vec4 color;
        Ref<TextureObject> texture = nullptr;

        Quad() {
            dimentions = Math::Vec3(0.0f);
            color = Math::Vec4(1.0f);
            texture = nullptr;
        }

        YAML::Node EncodeYAML() {
            YAML::Node node;
            node["dimentions"] = dimentions;
            node["color"] = color;
            if (texture != nullptr) {
                node["texture"] = texture->GetTextureIdentifier();
            }
            return node;
        }

        bool DecodeYAML(const YAML::Node& node, TextureLoader& texture_loader) {
            color = node["color"].as<Math::Vec4>();
            dimentions = node["dimentions"].as<Math::Vec3>();
            if (node["texture"]) {
                auto texture_opt = texture_loader.LoadTextureByIdentifier(node["texture"].as<std::string>());
                if (texture_opt.has_value()) {
                    texture = texture_opt.value();
                } else {
                    LA_CORE_ERROR("Could not find texture: {}", node["texture"].as<std::string>());
                    return false;
                }
            } else {
                texture = nullptr;
            }
            return true;
        }
    };
}
