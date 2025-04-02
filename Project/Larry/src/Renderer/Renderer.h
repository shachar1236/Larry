#include "../Log.h"
#include "Color.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RendererConfig.h"


namespace Larry {
    class Renderer {
        private:
            RendererConfig config;
            GLFWwindow* window;
        public:
            Renderer(RendererConfig config_);
            ~Renderer();

            bool ShouldClose();
            void Background(Color color);
            void UpdateFrame();

    };
}
