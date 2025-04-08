#include "Renderer.h"
#include "BufferObject.h"
#include "Log.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "TextureObject.h"
#include "VertexArrayObject.h"
#include "LarryMemory.h"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include <algorithm>
#include <string>
#include <unordered_map>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    LA_CORE_INFO("Windows resized to: ({}, {})", width, height);
}

namespace Larry {

    Renderer::Renderer(RendererConfig config_) : config(config_)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(config.window_width, config.window_height, config.window_name.c_str(), NULL, NULL);
        if (window == NULL)
        {
            LA_CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LA_CORE_ERROR("Failed to initialize GLAD" );
            glfwTerminate();
        }    

        glViewport(0, 0, config.viewport_width, config.viewport_height);
        if (config.resizable) {
            LA_CORE_INFO("Window is resizable!");
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // getting max texuters units
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_UNITS);
        LA_CORE_INFO("MAX_TEXTURE_UNITS = {}", MAX_TEXTURE_UNITS);
        white_texture = TextureObject::CreateWhiteTexture();
        currentlyUsedTextures.reserve(MAX_TEXTURE_UNITS);
        currentlyUsedTextures.push_back(white_texture);

        quad_texture_samplers = CreateScope<int[]>(MAX_TEXTURE_UNITS);
        for (int i = 0; i < MAX_TEXTURE_UNITS; i++) {
            quad_texture_samplers[i] = i;
        }

        // setting shaders
        Shader vertexShader = Shader("Shaders/basic_vertex_shader.glsl", ShaderType::ShaderVertex, {});
        std::unordered_map<std::string, std::string> fragment_shader_definations;
        fragment_shader_definations["MAX_TEXTURE_UNITS"] = std::to_string(MAX_TEXTURE_UNITS);
        Shader fragmentShader = Shader("Shaders/basic_fragment_shader.glsl", ShaderType::ShaderFragment, fragment_shader_definations);
        quad_shader_program = ShaderProgram(vertexShader, fragmentShader);
        quad_shader_program.AttachAndLink();

        quad_vao = CreateScope<VertexArrayObject>();
        quad_vao->Bind();
        
        int quads_vertices_buffer_size = MAX_TEXTURE_UNITS * 300 * sizeof(Vertex);
        quad_vertices_buffer = BufferObject<Vertex>(GL_ARRAY_BUFFER,  DrawType::DYNAMIC_DRAW);

        quad_vertices.reserve(quads_vertices_buffer_size);

        quad_vertices_buffer.Generate(quad_vertices.data(), quads_vertices_buffer_size);
        quad_vertices_buffer.SetAttributeOnVBO(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertices));
        quad_vertices_buffer.SetAttributeOnVBO(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colors));
        quad_vertices_buffer.SetAttributeOnVBO(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCords));
        quad_vertices_buffer.SetAttributeOnVBO(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureSlot));

        quad_vertices_buffer.SetAttributeOnVBO(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, modelCol1));
        quad_vertices_buffer.SetAttributeOnVBO(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, modelCol2));
        quad_vertices_buffer.SetAttributeOnVBO(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, modelCol3));
        quad_vertices_buffer.SetAttributeOnVBO(7, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, modelCol4));

        MAX_QUADS_NUMBER = (quads_vertices_buffer_size / 4) / sizeof(Vertex);
        LA_CORE_INFO("MAX_QUADS_NUMBER = {}", MAX_QUADS_NUMBER);
        quad_indices_buffer = BufferObject<unsigned int>(GL_ELEMENT_ARRAY_BUFFER,  DrawType::DYNAMIC_DRAW);

        quad_indices = CreateScope<unsigned int[]>(MAX_QUADS_NUMBER * 6);
        int offset = 0;
        for (int i = 0; i < MAX_QUADS_NUMBER * 6; i += 6) {
            quad_indices[i] = offset;
            quad_indices[i+1] = offset+1;
            quad_indices[i+2] = offset+3;
            quad_indices[i+3] = offset+1;
            quad_indices[i+4] = offset+2;
            quad_indices[i+5] = offset+3;
            offset += 4;
        }

        quad_indices_buffer.Generate(quad_indices.get(), MAX_QUADS_NUMBER * 6);

        quad_vao->Unbind();

    }

    Renderer::~Renderer() {
        LA_CORE_WARN("Terminating...");
        glfwTerminate();
    }


    bool Renderer::ShouldClose() {
        return glfwWindowShouldClose(window);
    }

    void Renderer::Background(Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::UpdateFrame() {
        FlushBatch();

        glfwSwapBuffers(window);
        glfwPollEvents();    

    }

    void Renderer::FlushBatch() {
        quad_vertices_buffer.ChangeData(0, quad_vertices.data(), quad_vertices.size() * sizeof(Vertex));
        // use our shader
        quad_shader_program.Use();
        // set textures
        quad_shader_program.SetUniform("ourTextures", quad_texture_samplers.get(), MAX_TEXTURE_UNITS);
        for (int i = 0; i < currentlyUsedTextures.size(); i++) {
            currentlyUsedTextures[i]->Activate(i);
            currentlyUsedTextures[i]->Bind();
        }
        // set view, projection
        quad_shader_program.SetUniform("u_View", View);
        quad_shader_program.SetUniform("u_Projection", Projection);
        // use our vao
        quad_vao->Bind();

        // draw the triangles
        glDrawElements(GL_TRIANGLES, quads_number * 6, GL_UNSIGNED_INT, 0);

        // resets triangles_vertices
        quad_vertices.clear();
        quads_number = 0;
    }

    void Renderer::DrawQuad(const float& x, const float& y, const float& width, const float& height) {
        if (quads_number >= MAX_QUADS_NUMBER) {
            LA_CORE_INFO("Reached MAX_QUADS_NUMBER flushing current batch");
            FlushBatch();
        }
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, CurrentShapeOptions.translation + glm::vec3(x, y, 0.0f));
        model = glm::rotate(model, CurrentShapeOptions.rotation, CurrentShapeOptions.rotation_axis);
        model = glm::scale(model, CurrentShapeOptions.scaling);

        Vertex ver1({}, {}, { CurrentShapeOptions.TextureCords[1][0], CurrentShapeOptions.TextureCords[1][1]}); // bottom right
        ver1.vertices = glm::vec3(0.5 * width, 0.5 * height, 0.0f);
        ver1.textureSlot = CurrentShapeOptions.CurrentTextureSlot;
        Vertex ver2( {}, {}, { CurrentShapeOptions.TextureCords[0][0], CurrentShapeOptions.TextureCords[0][1]}); // top right
        ver2.vertices = glm::vec3(0.5 * width, -0.5 * height, 0.0f);
        ver2.textureSlot = CurrentShapeOptions.CurrentTextureSlot;
        Vertex ver3({}, {}, { CurrentShapeOptions.TextureCords[3][0], CurrentShapeOptions.TextureCords[3][1]}); // top left
        ver3.vertices = glm::vec3(-0.5 * width, -0.5 * height, 0.0f);
        ver3.textureSlot = CurrentShapeOptions.CurrentTextureSlot;
        Vertex ver4({}, {}, { CurrentShapeOptions.TextureCords[2][0], CurrentShapeOptions.TextureCords[2][1]}); // bottom left
        ver4.vertices = glm::vec3(-0.5 * width, 0.5 * height, 0.0f);
        ver4.textureSlot = CurrentShapeOptions.CurrentTextureSlot;
        for (int i = 0; i < 4; i++) {
            ver1.colors[i] = CurrentShapeOptions.FillColors[i];
            ver2.colors[i] = CurrentShapeOptions.FillColors[i];
            ver3.colors[i] = CurrentShapeOptions.FillColors[i];
            ver4.colors[i] = CurrentShapeOptions.FillColors[i];
        }

        for (int i = 0; i < 4; i++) {
            ver1.modelCol1 = model[0];
            ver2.modelCol1 = model[0];
            ver3.modelCol1 = model[0];
            ver4.modelCol1 = model[0];

            ver1.modelCol2 = model[1];
            ver2.modelCol2 = model[1];
            ver3.modelCol2 = model[1];
            ver4.modelCol2 = model[1];

            ver1.modelCol3 = model[2];
            ver2.modelCol3 = model[2];
            ver3.modelCol3 = model[2];
            ver4.modelCol3 = model[2];

            ver1.modelCol4 = model[3];
            ver2.modelCol4 = model[3];
            ver3.modelCol4 = model[3];
            ver4.modelCol4 = model[3];
        }

        quad_vertices.push_back(ver1);
        quad_vertices.push_back(ver2);
        quad_vertices.push_back(ver3);
        quad_vertices.push_back(ver4);

        quads_number++;
        CurrentShapeOptions = ShapeOptions();
    }


    void Renderer::InitializeOrthographicProjection(const float& left, const float& right, const float& bottom, const float& top, const float& nearDist, const float& farDist) {
        Projection = glm::ortho(left, right, bottom, top, nearDist, farDist);
    }

    void Renderer::InitializePrespectiveProjection(const float& fov, const float& aspectRatio, const float& near, const float& far) {
        Projection = glm::perspective(fov, aspectRatio, near, far);
    }

    void Renderer::Fill(float r, float g, float b, float a) {
        CurrentShapeOptions.FillColors[0] = r;
        CurrentShapeOptions.FillColors[1] = g;
        CurrentShapeOptions.FillColors[2] = b;
        CurrentShapeOptions.FillColors[3] = a;
    }

    void Renderer::Texture(const Ref<TextureObject>& texture) {
        auto found = std::find(currentlyUsedTextures.begin(), currentlyUsedTextures.end(), texture);
        if (found != currentlyUsedTextures.end()) {
            int index = found - currentlyUsedTextures.begin();
            CurrentShapeOptions.CurrentTextureSlot = index;
        } else {
            if (currentlyUsedTextures.size() >= MAX_TEXTURE_UNITS) {
                LA_CORE_INFO("Reached MAX_TEXTURE_UNITS flushing current batch");
                ShapeOptions options = CurrentShapeOptions;
                FlushBatch();
                currentlyUsedTextures.erase(currentlyUsedTextures.begin() + 1, currentlyUsedTextures.end());
                CurrentShapeOptions = options;
            }
            currentlyUsedTextures.push_back(texture);
            CurrentShapeOptions.CurrentTextureSlot = currentlyUsedTextures.size() - 1;
        }
    }

    void Renderer::TextureCords(const float cords[4][2]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2; j++) {
                CurrentShapeOptions.TextureCords[i][j] = cords[i][j];
            }
        }
    }

    void Renderer::Translate(const glm::vec3& amount) {
        CurrentShapeOptions.translation = amount;
    }

    void Renderer::Scale(const glm::vec3& amount) {
        CurrentShapeOptions.scaling = amount;
    }

    void Renderer::Rotate(const float& degrees) {
        CurrentShapeOptions.rotation = degrees; 
    }

    void Renderer::Rotate(const float& degrees, const glm::vec3& axis) {
        CurrentShapeOptions.rotation = degrees; 
        CurrentShapeOptions.rotation_axis = axis;
    }
}
