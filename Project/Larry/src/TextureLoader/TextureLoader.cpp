#include "TextureLoader/TextureLoader.h"
#include "Utils/LarryMemory.h"
#include "TextureLoader/TextureObject.h"
#include <cstddef>
#include <functional>
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
        
        textures[hs] = CreateRef<TextureObject>(path, config);
        return textures[hs];
    }
}
