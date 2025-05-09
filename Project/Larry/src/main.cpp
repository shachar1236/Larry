#include "Application.h"
#include "Log.h"
#include "gl.h"
#ifdef LARRY_ENABLE_TESTING
#include "ECS/ecs_test.hpp"
#endif

int main()
{
    Larry::Log::Init();
    Larry::ECS::Init();

#ifdef LARRY_ENABLE_TESTING
    LA_CORE_INFO("Testing ECS");
    Larry::ECS::TestECS();
#endif

    Larry::Application* app = Larry::Application::GetApplication();
    app->Run();
    delete app;
    return 0;
}
