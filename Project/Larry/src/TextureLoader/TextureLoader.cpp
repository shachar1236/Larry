#include "common.h"
#include "TextureLoader/TextureLoader.h"
#include "Utils/LarryMemory.h"
#include "TextureLoader/TextureObject.h"
#include <cstddef>
#include <functional>
#include <optional>
#include <string>

namespace Larry {
    TextureLoader::TextureLoader() {

    }

    TextureLoader::~TextureLoader() {

    }

    Ref<TextureObject> TextureLoader::LoadTexture(const std::string& path, const TextureConfig& config) {
        std::size_t hs = std::hash<std::string>{}(path) ^ std::hash<TextureConfig>{}(config);
        auto texture_it = textures.find(hs);
        if (texture_it != textures.end()) {
            return texture_it->second;
        }
        
        Ref<TextureObject> t = CreateRef<TextureObject>(path, config);
        textures[hs]  = t;
        return t;
    }

    void TextureLoader::SetTextureIdentifier(const Ref<TextureObject>& texture, const std::string& identifier) {
        identifier_to_texture[identifier] = texture;
        texture->identifier = identifier;
    }

    std::optional<Ref<TextureObject>> TextureLoader::LoadTextureByIdentifier(const std::string& identifier) {
        auto res = identifier_to_texture.find(identifier);
        if (res == identifier_to_texture.end()) {
            return std::nullopt;
        }
        return res->second;
    }

}
