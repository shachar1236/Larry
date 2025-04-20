#include "TestScript.h"
#include "Scripts/Scripts.h"
#include "Utils/Log.h"
#include "ECS/World.hpp"

namespace Larry {

    TestScript::TestScript(const Ref<ECS::World>& world_) : Scripts::Script(world_) {

    }

    TestScript::~TestScript() {

    }

    void TestScript::OnCreate() {
        LA_INFO("TestScript OnCreate!!!!!");
    }

    void TestScript::OnUpdate(double deltaTime) {
        LA_INFO("TestScript OnUpdate");
    }

    void TestScript::HandleEvent(const Ref<Event>&) {
        LA_INFO("TestScript HandleEvent");
    }
}
