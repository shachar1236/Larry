#pragma once
#include "Log.h"
#include "BufferObject.h"
#include "Color.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "LarryMemory.h"
#include <vector>
#include "RendererConfig.h"
#include "ShaderProgram.h"
#include "TextureObject.h"
#include "VertexArrayObject.h"

namespace Larry {

    struct Vertex {
        float vertices[3];
        float colors[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float textureCords[2];
        float textureSlot;
    };

    struct ShapeOptions {
        float FillColors[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float TextureCords[4][2] = {
            { 1.0f, 1.0f },
            { 1.0f, 0.0f },
            { 0.0f, 0.0f },
            { 0.0f, 1.0f },
        };
        int CurrentTextureSlot = 0;
    };

    class Renderer {
        private:
            RendererConfig config;
            GLFWwindow* window;
            int MAX_TEXTURE_UNITS;
            unsigned int MAX_QUADS_NUMBER;
            unsigned int quads_number = 0;

            ShaderProgram quad_shader_program;
            Scope<int[]> quad_texture_samplers;

            Scope<VertexArrayObject> quad_vao;

            std::vector<Vertex> quad_vertices;
            BufferObject<Vertex> quad_vertices_buffer;

            Scope<unsigned int[]> quad_indices;
            BufferObject<unsigned int> quad_indices_buffer;

            Ref<TextureObject> white_texture;
            std::vector<Ref<TextureObject>> currentlyUsedTextures;
        public:
            ShapeOptions CurrentShapeOptions;

            Renderer(RendererConfig config_);
            ~Renderer();

            // bool ShouldClose() - returns true if someone clicked on close
            bool ShouldClose();
            // void Background(Color) - Paints the background with the color provided
            void Background(Color color);
            // void UpdateFrame() - Draw the frame to the screen
            void UpdateFrame();
            // void FlushBatch() - draws the current batch to the screen
            void FlushBatch();

            void DrawQuad(const float& x, const float& y, const float& width, const float& height);
            /* void DrawTriangle(const float& x, const float& y, const float& width, const float& height); */

            // Setting next draw options
            void Fill(float r, float g, float b, float a);
            void Texture(const Ref<TextureObject>& texture);
            void TextureCords(const float cords[4][2]);
    };
}
