#pragma once
#include "../Log.h"
#include "BufferObject.h"
#include "Color.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "RendererConfig.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"

namespace Larry {

    struct Vertex {
        float vertices[3];
    };

    class Renderer {
        private:
            RendererConfig config;
            GLFWwindow* window;
            int MAX_TEXURE_UNITS;

            ShaderProgram quad_shader_program;

            std::unique_ptr<VertexArrayObject> quad_vao;

            std::vector<Vertex> quads_vertices;
            BufferObject<Vertex> quads_vertices_buffer;

            std::vector<unsigned int> quads_indices;
            BufferObject<unsigned int> quads_indices_buffer;
        public:
            Renderer(RendererConfig config_);
            ~Renderer();

            // bool ShouldClose() - returns true if someone clicked on close
            bool ShouldClose();
            // void Background(Color) - Paints the background with the color provided
            void Background(Color color);
            // void UpdateFrame() - Draw the frame to the screen
            void UpdateFrame();

            void DrawQuad(const float& x, const float& y, const float& width, const float& height);
    };
}
