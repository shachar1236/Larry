#include "Log.h"
#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ECS/ecs_test.hpp"

int main() {
    Larry::Log::Init();

    LA_CORE_INFO("Testing ECS");
    Larry::ECS::TestECS();
#ifdef LARRY_ENABLE_TESTING
#endif

    Larry::Application* app = Larry::Application::GetApplication();
    app->Run();
    delete app;
    return 0;
}
