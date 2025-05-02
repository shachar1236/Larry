#pragma once

#include "Utils/LarryMemory.h"
#include <string>

namespace Larry {
    enum TextureWrappingOptions {
        REPEAT = 0x2901,
        MIRRORED_REPEAT  = 0x8370,
        CLAMP_TO_EDGE = 0x812F,
        CLAMP_TO_BORDER = 0x812D
    };

    enum TextureFilterOptions {
        NEAREST = 0x2600,
        LINEAR = 0x2601
    };

    enum MipmapFilterOptions {
        NEAREST_MIPMAP_NEAREST = 0x2700,
        LINEAR_MIPMAP_NEAREST = 0x2701,
        NEAREST_MIPMAP_LINEAR = 0x2702,
        LINEAR_MIPMAP_LINEAR = 0x2703
    };

    struct TextureConfig {
        bool CreateMipmap = true;
        enum TextureWrappingOptions TextureWrappingS = TextureWrappingOptions::REPEAT;
        enum TextureWrappingOptions TextureWrappingT = TextureWrappingOptions::REPEAT;
        enum TextureFilterOptions TextureFilterMin = TextureFilterOptions::LINEAR;
        enum TextureFilterOptions TextureFilterMag = TextureFilterOptions::LINEAR;
        enum MipmapFilterOptions MipmapFilterMin = MipmapFilterOptions::LINEAR_MIPMAP_LINEAR;
    };

    class TextureObject {
        private:
            unsigned int texture;
            int width, height, nrChannels;
        public:
            TextureObject() {}
            TextureObject(const std::string& path, const TextureConfig& config);
            ~TextureObject();

            void Bind();
            void Activate(const int& slot);

            unsigned int GetTexture() { return texture; }
            int GetWidth() { return width; }
            int GetHeight() { return height; }
            int GetNrChannels() { return nrChannels; }
            bool operator==(const TextureObject& other) {
                return this->texture == other.texture;
            }

            static Ref<TextureObject> CreateWhiteTexture();
    };
}

template<>
struct std::hash<Larry::TextureConfig>
{
    std::size_t operator()(const Larry::TextureConfig& s) const noexcept
    {
        std::size_t h1 = std::hash<bool>{}(s.CreateMipmap);
        std::size_t h2 = std::hash<int>{}(s.TextureWrappingS);
        std::size_t h3 = std::hash<int>{}(s.MipmapFilterMin);
        std::size_t h4 = std::hash<int>{}(s.TextureFilterMag);
        std::size_t h5 = std::hash<int>{}(s.TextureFilterMin);
        std::size_t h6 = std::hash<int>{}(s.TextureWrappingT);
        return h1 ^ h2 ^ h3 ^ h4 ^ h5 ^ h6;
    }
};

