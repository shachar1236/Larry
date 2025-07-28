#pragma once

#include <yaml-cpp/yaml.h>
#include "TextureLoader/TextureObject.h"
#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>

namespace Larry {
    class TextureLoader {
    public:
        std::unordered_map<std::size_t, TextureObject*> textures;
        std::unordered_map<std::string, TextureObject*> identifier_to_texture;

        TextureLoader();
        ~TextureLoader();

        TextureObject* LoadTexture(const std::string& path, const TextureConfig& config);
        void SetTextureIdentifier(TextureObject* texture, const std::string& identifier);
        std::optional<TextureObject*> LoadTextureByIdentifier(const std::string& identifier);

    };
}

namespace YAML {

    template<>
    struct convert<Larry::TextureLoader> {
        static Node encode(const Larry::TextureLoader& rhs) {
            Node node;
            for (auto&& t : rhs.textures) {
                std::string identifier;
                identifier = t.second->GetTextureIdentifier();
                node[identifier] = convert<Larry::TextureConfig>::encode(t.second->GetConfig());
                node[identifier]["path"] = t.second->GetPath();
            }
            return node;
        }

        static bool decode(const Node& node, Larry::TextureLoader& rhs) {
            if(!node.IsMap()) {
                return false;
            }

            for (auto&& texture : node) {
                Larry::TextureConfig config = texture.second.as<Larry::TextureConfig>();
                Larry::TextureObject* t = rhs.LoadTexture(texture.second["path"].as<std::string>(), config);
                rhs.SetTextureIdentifier(t, texture.first.as<std::string>());
            }
            return true;
        }
    };
}
