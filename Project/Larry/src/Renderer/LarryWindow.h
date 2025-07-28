#pragma once

// #include "GLFW/glfw3.h"
#include "gl.h"
#include "LarryApi.h"
#include <string>
#include <yaml-cpp/yaml.h>

namespace Larry {
    struct WindowConfig {
        std::string title = "Basic title";
        
        int window_width = 800;
        int window_height = 600;

        bool fullscreen = false;
        bool maximized = false;
    };

    enum CursorMode {
        Hidden = GLFW_CURSOR_HIDDEN,
        Captured = GLFW_CURSOR_CAPTURED,
        Normal = GLFW_CURSOR_NORMAL,
    };

    class LarryWindow {
        private:
            WindowConfig config;
            GLFWwindow* window;
        public:
            LarryWindow(const WindowConfig& config_);
            ~LarryWindow();

            GLFWwindow* GetWindow() { return window; }
            // bool ShouldClose() - returns true if someone clicked on close
            bool ShouldClose();

            void SetFulllscreen(int xpos, int ypos, int width, int height, int refreshRate);
            void SetWindowed(int xpos, int ypos, int width, int height, int refreshRate);

            void UpdateResulotion(int width, int height);
            
            void SetWindowSizeLimits(int minwidth, int minheight, int maxwidth, int maxheight);
            void SetWindowIcon(GLFWimage* images, int images_number);

            void Maximize();
            void Unmaximize();

            void SetCursorMode(CursorMode mode);
    };
}

namespace YAML {

    template<>
    struct convert<Larry::WindowConfig> {
        static Node encode(const Larry::WindowConfig& rhs) {
            Node node;
            node["title"] = rhs.title;
            node["width"] = rhs.window_width;
            node["height"] = rhs.window_height;
            node["fullscreen"] = rhs.fullscreen;
            node["maximized"] = rhs.maximized;
            return node;
        }

        static bool decode(const Node& node, Larry::WindowConfig& rhs) {
            if(!node.IsMap()) {
                return false;
            }
            rhs.title = node["title"].as<std::string>();
            rhs.window_width = node["width"].as<int>();
            rhs.window_height = node["height"].as<int>();
            rhs.fullscreen = node["fullscreen"].as<bool>();
            rhs.maximized = node["maximized"].as<bool>();
            return true;
        }
    };
}
