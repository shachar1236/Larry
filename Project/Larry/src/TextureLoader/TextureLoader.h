#pragma once

#include "TextureLoader/TextureObject.h"
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>

namespace Larry {
    class TextureLoader {
    private:
        std::unordered_map<std::size_t, Ref<TextureObject>> textures;

    public:
        TextureLoader();
        ~TextureLoader();

        Ref<TextureObject> LoadTexture(const std::string& path, const TextureConfig& config);
    };
}
