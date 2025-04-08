#pragma once

#include <string>
namespace Larry {
    struct RendererConfig {
        int window_width = 800;
        int window_height = 600;
        int viewport_width = 800;
        int viewport_height = 600;
        std::string window_name = "test";
    };
}
