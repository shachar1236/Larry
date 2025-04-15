#include "Log.h"
#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ECS/ecs_test.hpp"

int main() {
    Larry::Log::Init();

#ifdef LARRY_ENABLE_TESTING
    LA_CORE_INFO("Testing ECS");
    Larry::ECS::TestECS();
#endif

    Larry::Application* app = Larry::Application::GetApplication();
    app->Run();
    delete app;
    return 0;
}
