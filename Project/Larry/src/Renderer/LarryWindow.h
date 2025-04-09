#pragma once

#include "GLFW/glfw3.h"
#include <string>

namespace Larry {
    struct WindowConfig {
        std::string title = "Basic title";
        
        int window_width = 800;
        int window_height = 600;

        bool fullscreen = false;
        bool maximized = false;
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
    };
}
