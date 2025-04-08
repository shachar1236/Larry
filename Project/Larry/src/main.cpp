#include "Log.h"
#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    Larry::Log::Init();
    Larry::Application* app = Larry::Application::GetApplication();
    app->Run();
    delete app;
    return 0;
}
