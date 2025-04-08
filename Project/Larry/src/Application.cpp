#include "Application.h"
#include "GLFW/glfw3.h"
#include "LarryMemory.h"
#include "TextureObject.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
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
        renderer.InitializeOrthographicProjection(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        renderer.View = glm::translate(renderer.View, glm::vec3(0.0f, 0.0f, -3.0f));
        while(!renderer.ShouldClose())
        {
            double time = glfwGetTime() ;
            renderer.Background(Color{0.1f, 0.1f, 0.3f, 1});

            /* renderer.Texture(wall); */
            /* renderer.Scale(glm::vec3(sin(time)+2.0f * 3.0f, 1.01f, 1.0f)); */
            /* renderer.Rotate(cos(time) * 3.14); */
            /* renderer.Translate(glm::vec3(0.25f, -0.25f, 0.0f)); */
            /* renderer.Translate(glm::vec3(0.0, 0.0, -0.5f)); */
            /* renderer.Scale(glm::vec3(1.0f, 2.0f, 1.0f)); */
            /* renderer.Translate(glm::vec3(400.0f, 300.0f, 0.0f)); */
            renderer.Fill(1, 1, 1, 1);
            renderer.DrawQuad(200.0f, 300.0f, 200.0f, 200.0f);

            /* renderer.Texture(face); */
            /* renderer.Rotate((time)); */
            /* renderer.Translate(glm::vec3(-0.85f, -0.85f, 0.0f)); */
            /* renderer.DrawQuad(0.2f, 0.7f, 0.3f, 0.3f); */

            renderer.UpdateFrame();
        }
    }
}
