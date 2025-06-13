#pragma once

#include "Log.h"
#include "Math/Math.h"
#include "TextureLoader.h"
#include "TextureLoader/TextureObject.h"
#include "yaml-cpp/node/node.h"
#include <yaml-cpp/yaml.h>

namespace Larry {
    struct Background {
        Math::Vec4 color;
        Ref<TextureObject> texture;

        Background() {
            color = Math::Vec4(1.0f);
            texture = nullptr;
        }

        YAML::Node EncodeYAML() {
            YAML::Node node;
            node["color"] = color;
            node["texture"] = texture->GetTextureIdentifier();
            return node;
        }

        bool DecodeYAML(const YAML::Node& node, TextureLoader& texture_loader) {
            color = node["color"].as<Math::Vec4>();
            auto texture_opt = texture_loader.LoadTextureByIdentifier(node["texture"].as<std::string>());
            if (texture_opt.has_value()) {
                texture = texture_opt.value();
            } else {
                LA_CORE_ERROR("Could not find texture: {}", node["texture"].as<std::string>());
                return false;
            }
            return true;
        }
    };

}
