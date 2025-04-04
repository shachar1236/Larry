#include "Application.h"

namespace Larry {
    Application::Application() : renderer(Renderer(rendererConfig)) {
    }

    Application::~Application() {

    }

    void Application::Run() {
        while(!renderer.ShouldClose())
        {
            renderer.Background(Color{1, 1, 1, 1});
            renderer.DrawQuad(-0.5f, 0.5f, 1.0f, 1.0f);
            renderer.UpdateFrame();
        }
    }
}
