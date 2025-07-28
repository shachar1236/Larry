#pragma once

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
}
