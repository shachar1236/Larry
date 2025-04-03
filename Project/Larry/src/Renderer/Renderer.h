#pragma once
#include "../Log.h"
#include "BufferObject.h"
#include "Color.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "RendererConfig.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"


namespace Larry {
    class Renderer {
        private:
            RendererConfig config;
            GLFWwindow* window;
            ShaderProgram basic_shader_program;

            BufferObject<float> vertex_buffer;
            BufferObject<unsigned int> indices_buffer;
            std::unique_ptr<VertexArrayObject> vao1;
        public:
            Renderer(RendererConfig config_);
            ~Renderer();

            // bool ShouldClose() - returns true if someone clicked on close
            bool ShouldClose();
            // void Background(Color) - Paints the background with the color provided
            void Background(Color color);
            // void UpdateFrame() - Draw the frame to the screen
            void UpdateFrame();

    };
}
