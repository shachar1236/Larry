#pragma once

#include <yaml-cpp/yaml.h>
#include "Utils/LarryMemory.h"
#include <string>

#define CASE_VALUE(x) case x: return #x;
#define RETURN_IF_EQUAL(x) if (str == #x) { return x; }

namespace Larry {
    enum TextureWrappingOptions {
        REPEAT = 0x2901,
        MIRRORED_REPEAT  = 0x8370,
        CLAMP_TO_EDGE = 0x812F,
        CLAMP_TO_BORDER = 0x812D
    };

    std::string EncodeTextureWrappingOptions(enum TextureWrappingOptions value) {
        switch (value) {
            CASE_VALUE(REPEAT);
            CASE_VALUE(MIRRORED_REPEAT);
            CASE_VALUE(CLAMP_TO_EDGE);
            CASE_VALUE(CLAMP_TO_BORDER);
        }
    }

     enum TextureWrappingOptions DecodeTextureWrappingOptions(std::string str) {
        RETURN_IF_EQUAL(REPEAT);
        RETURN_IF_EQUAL(MIRRORED_REPEAT);
        RETURN_IF_EQUAL(CLAMP_TO_EDGE);
        RETURN_IF_EQUAL(CLAMP_TO_BORDER);

        return REPEAT;
    }

    enum TextureFilterOptions {
        NEAREST = 0x2600,
        LINEAR = 0x2601
    };

    std::string EncodeTextureFilterOptions(enum TextureFilterOptions value) {
        switch (value) {
            CASE_VALUE(NEAREST);
            CASE_VALUE(LINEAR);
        }
    }

    enum TextureFilterOptions DecodeTextureFilterOptions(std::string str) {
        RETURN_IF_EQUAL(NEAREST);
        RETURN_IF_EQUAL(LINEAR);

        return NEAREST;
    }

    enum MipmapFilterOptions {
        NEAREST_MIPMAP_NEAREST = 0x2700,
        LINEAR_MIPMAP_NEAREST = 0x2701,
        NEAREST_MIPMAP_LINEAR = 0x2702,
        LINEAR_MIPMAP_LINEAR = 0x2703
    };

    std::string EncodeMipmapFilterOptions(enum MipmapFilterOptions value) {
        switch (value) {
            CASE_VALUE(NEAREST_MIPMAP_NEAREST);
            CASE_VALUE(LINEAR_MIPMAP_NEAREST);
            CASE_VALUE(NEAREST_MIPMAP_LINEAR);
            CASE_VALUE(LINEAR_MIPMAP_LINEAR);
        }
    }

    enum MipmapFilterOptions DecodeMipmapFilterOptions(std::string str) {
        RETURN_IF_EQUAL(NEAREST_MIPMAP_NEAREST);
        RETURN_IF_EQUAL(LINEAR_MIPMAP_NEAREST);
        RETURN_IF_EQUAL(NEAREST_MIPMAP_LINEAR);
        RETURN_IF_EQUAL(LINEAR_MIPMAP_LINEAR);

        return LINEAR_MIPMAP_LINEAR;
    }

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
            std::string identifier;
            std::string path;
            TextureConfig config;
        public:
            TextureObject() {}
            TextureObject(const std::string& path_, const TextureConfig& config_);
            ~TextureObject();

            void Bind();
            void Activate(const int& slot);

            unsigned int GetTexture() { return texture; }
            int GetWidth() { return width; }
            int GetHeight() { return height; }
            int GetNrChannels() { return nrChannels; }
            std::string GetPath() { return path; }
            TextureConfig GetConfig() { return config; }
            std::string GetTextureIdentifier() { return identifier; }
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

namespace YAML {

    template<>
    struct convert<Larry::TextureConfig> {
        static Node encode(const Larry::TextureConfig& rhs) {
            Node node;
            node["CreateMipmap"] = rhs.CreateMipmap;
            node["TextureWrappingS"] = Larry::EncodeTextureWrappingOptions(rhs.TextureWrappingS);
            node["TextureWrappingT"] = Larry::EncodeTextureWrappingOptions(rhs.TextureWrappingT);
            node["TextureFilterMin"] = Larry::EncodeTextureFilterOptions(rhs.TextureFilterMin);
            node["TextureFilterMag"] = Larry::EncodeTextureFilterOptions(rhs.TextureFilterMag);
            node["MipmapFilterMin"] = Larry::EncodeMipmapFilterOptions(rhs.MipmapFilterMin);
            return node;
        }

        static bool decode(const Node& node, Larry::TextureConfig& rhs) {
            if(!node.IsMap()) {
                return false;
            }

            rhs.CreateMipmap = node["CreateMipmap"].as<bool>();
            rhs.TextureWrappingS = Larry::DecodeTextureWrappingOptions(node["TextureWrappingS"].as<std::string>());
            rhs.TextureWrappingT = Larry::DecodeTextureWrappingOptions(node["TextureWrappingT"].as<std::string>());
            rhs.TextureFilterMin = Larry::DecodeTextureFilterOptions(node["TextureFilterMin"].as<std::string>());
            rhs.TextureFilterMag = Larry::DecodeTextureFilterOptions(node["TextureFilterMag"].as<std::string>());
            rhs.MipmapFilterMin = Larry::DecodeMipmapFilterOptions(node["MipmapFilterMin"].as<std::string>());

            return true;
        }
    };
}
