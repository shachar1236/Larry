#include "Renderer.h"

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
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
}
