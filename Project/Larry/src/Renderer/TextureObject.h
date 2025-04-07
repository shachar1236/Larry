#pragma once

#include "LarryMemory.h"
#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


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
