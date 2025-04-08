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
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"

namespace Larry {

    struct Vertex {
        glm::vec3 vertices;
        float colors[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float textureCords[2];
        float textureSlot;
        glm::vec4 modelCol1;
        glm::vec4 modelCol2;
        glm::vec4 modelCol3;
        glm::vec4 modelCol4;
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
        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
        float rotation = 0.0f;
        glm::vec3 rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 scaling = glm::vec3(1.0f, 1.0f, 1.0f);
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
            glm::mat4 View = glm::mat4(1.0f);
            glm::mat4 Projection = glm::mat4(1.0f);

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

            // initializing variables
            void InitializeOrthographicProjection(const float& left, const float& right, const float& bottom, const float& top, const float& nearDist, const float& farDist);
            void InitializePrespectiveProjection(const float& fov, const float& aspectRatio, const float& near, const float& far);

            // Setting next draw options
            void Fill(float r, float g, float b, float a);
            void Texture(const Ref<TextureObject>& texture);
            void TextureCords(const float cords[4][2]);
            void Translate(const glm::vec3& amount);
            void Scale(const glm::vec3& amount);
            void Rotate(const float& degrees);
            void Rotate(const float& degrees, const glm::vec3& axis);
    };
}
