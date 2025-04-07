#include "TextureObject.h"
#include "BufferObject.h"
#include "LarryMemory.h"
#include "Log.h"
#include <cstdlib>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Larry {
    TextureObject::TextureObject(const std::string& path, const TextureConfig& config) {
        glGenTextures(1, &texture); 
        Bind();

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.TextureWrappingS);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.TextureWrappingT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.TextureFilterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.TextureFilterMag);

        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0); 
        if (!data) {
            LA_CORE_ERROR("Could not load texture ", path);
            exit(0);
        }

        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        }
        else if (nrChannels == 3) {
            format = GL_RGB;
        }
        else if (nrChannels == 4) {
            LA_CORE_INFO("4 channels texture detected!");
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if (config.CreateMipmap) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.MipmapFilterMin);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        stbi_image_free(data);

        LA_CORE_INFO("Created texture from: {}, ({}, {}, {})", path, width, height, nrChannels);
    }

    TextureObject::~TextureObject() {
        glDeleteTextures(1, &texture);
    }


    void TextureObject::Bind() {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void TextureObject::Activate(const int& slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    Ref<TextureObject> TextureObject::CreateWhiteTexture() {
        Ref<TextureObject> white = CreateRef<TextureObject>();
        glGenTextures(1, &white->texture); 
        white->Bind();

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        uint32_t color = 0xffffffff;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
        white->width = 1;
        white->height = 1;

        return white;
    }
}
