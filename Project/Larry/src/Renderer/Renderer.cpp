#include "Renderer.h"
#include "BufferObject.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include <cstddef>
#include <memory>

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

        // getting max texuters units
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXURE_UNITS);

        /* float vertices[] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f,   // top left 

             0.7f, 0.0f, 0.0f,
             0.9f, 0.0f, 0.0f,
             0.7f, 0.7f, 0.0f,
             0.9f, 0.7f, 0.0f,
        }; 
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3,    // second triangle
            
            4, 5, 6,
            5, 6, 7,
        };  */

        // setting shaders
        Shader vertexShader = Shader("Shaders/basic_vertex_shader.glsl", ShaderType::ShaderVertex);
        Shader fragmentShader = Shader("Shaders/basic_fragment_shader.glsl", ShaderType::ShaderFragment);
        quad_shader_program = ShaderProgram(vertexShader, fragmentShader, {});
        quad_shader_program.AttachAndLink();

        quad_vao = std::make_unique<VertexArrayObject>();
        quad_vao->Bind();

        
        int quads_vertices_buffer_size = MAX_TEXURE_UNITS * 100 * sizeof(Vertex);
        quads_vertices_buffer = BufferObject<Vertex>(GL_ARRAY_BUFFER,  DrawType::DYNAMIC_DRAW);

        quads_vertices.reserve(quads_vertices_buffer_size);

        quads_vertices_buffer.Generate(quads_vertices.data(), quads_vertices_buffer_size);
        quads_vertices_buffer.SetAttributeOnVBO(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertices));

        int quads_indices_buffer_size = quads_vertices_buffer_size / 4;
        quads_indices_buffer = BufferObject<unsigned int>(GL_ELEMENT_ARRAY_BUFFER,  DrawType::DYNAMIC_DRAW);

        quads_indices.reserve(quads_indices_buffer_size);

        quads_indices_buffer.Generate(quads_indices.data(), quads_vertices_buffer_size);

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
        quads_vertices_buffer.ChangeData(0, quads_vertices.data(), quads_vertices.size() * sizeof(Vertex));
        quads_indices_buffer.ChangeData(0, quads_indices.data(), quads_indices.size() * sizeof(unsigned int));
        // use our shader
        quad_shader_program.Use();
        // use our vao
        quad_vao->Bind();

        // draw the triangles
        glDrawElements(GL_TRIANGLES, quads_indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    

        quads_vertices.clear();
        quads_indices.clear();
    }

    void Renderer::DrawQuad(const float& x, const float& y, const float& width, const float& height) {
        Vertex ver1({x, y, 0.0f});
        Vertex ver2({x+width, y, 0.0f});
        Vertex ver3({x+width, y-height, 0.0f});
        Vertex ver4({x, y-height, 0.0f});

        quads_vertices.push_back(ver1);
        quads_vertices.push_back(ver2);
        quads_vertices.push_back(ver3);
        quads_vertices.push_back(ver4);

        unsigned int last_number = 0;
        if (quads_indices.size() > 0) {
            last_number = quads_indices.back();
        }

        quads_indices.push_back(last_number+0);
        quads_indices.push_back(last_number+1);
        quads_indices.push_back(last_number+2);
        quads_indices.push_back(last_number+0);
        quads_indices.push_back(last_number+2);
        quads_indices.push_back(last_number+3);
    }
}
