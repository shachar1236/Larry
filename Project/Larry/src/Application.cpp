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
            renderer.UpdateFrame();
        }
    }
}
