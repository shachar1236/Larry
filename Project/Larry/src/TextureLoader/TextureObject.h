#pragma once

#include "Utils/LarryMemory.h"
#include <string>
#include "glad/glad.h"


namespace Larry {
    enum TextureWrappingOptions {
        REPEAT = GL_REPEAT,
        MIRRORED_REPEAT  = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };

    enum TextureFilterOptions {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR
    };

    enum MipmapFilterOptions {
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
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

