#include "Application.h"
#include "GLFW/glfw3.h"
#include "LarryMemory.h"
#include "TextureObject.h"
#include <cmath>

namespace Larry {
    Application::Application() : renderer(Renderer(rendererConfig)) {
        TextureConfig config;
        config.CreateMipmap = false;
        face = CreateRef<TextureObject>("media/textures/awesomeface.png", config);
        test = CreateRef<TextureObject>("media/textures/test.jpg", TextureConfig{});
        wall = CreateRef<TextureObject>("media/textures/wall.jpg", TextureConfig{});
    }

    Application::~Application() {

    }

    void Application::Run() {
        while(!renderer.ShouldClose())
        {
            double time = glfwGetTime() ;
            renderer.Background(Color{0.1f, 0.1f, 0.3f, 1});
            renderer.Texture(face);
            renderer.Fill(0.2, 0.8, 0.5, 1.0);
            renderer.DrawQuad(-0.5f, 0.5f, 0.5f, 0.5f);
            renderer.Texture(wall);
            renderer.DrawQuad(0.1f, 0.0f, 0.5f, 0.5f);
            renderer.Texture(test);
            renderer.DrawQuad(-0.7f, sin(time), 0.5f, 0.5f);
            renderer.UpdateFrame();
        }
    }
}
