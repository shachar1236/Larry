#include "Renderer.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"
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

        float vertices[] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left 
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };  

        // setting shaders
        Shader vertexShader = Shader("Shaders/basic_vertex_shader.glsl", ShaderType::ShaderVertex);
        Shader fragmentShader = Shader("Shaders/basic_fragment_shader.glsl", ShaderType::ShaderFragment);
        basic_shader_program = ShaderProgram(vertexShader, fragmentShader);
        basic_shader_program.AttachAndLink();

        vao1 = std::make_unique<VertexArrayObject>();
        vao1->Bind();

        vertex_buffer = BufferObject<float>(GL_ARRAY_BUFFER, vertices, DrawType::STATIC_DRAW, 
                BufferObjectAtrributes{0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0}
            );
        vertex_buffer.Generate();
        vertex_buffer.SetAttributeOnVBO();

        indices_buffer = BufferObject<unsigned int>(GL_ELEMENT_ARRAY_BUFFER, indices, DrawType::STATIC_DRAW, BufferObjectAtrributes{});
        indices_buffer.Generate();
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
        // use our shader
        basic_shader_program.Use();
        // use our vao
        vao1->Bind();

        // draw the triangles
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
}
