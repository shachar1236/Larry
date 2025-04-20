#include "TestScript.h"
#include "Scripts/Scripts.h"
#include "Utils/Log.h"
#include "ECS/World.hpp"

namespace Larry {

    TestScript::TestScript(const Ref<ECS::World>& world_) : Scripts::Script(world_) {

    }

    TestScript::~TestScript() {

    }

    void TestScript::OnCreate(const ECS::Entity& entity) {
        LA_INFO("TestScript OnCreate!!!!!");
    }

    void TestScript::OnUpdate(const ECS::Entity& entity, double deltaTime) {
        LA_INFO("TestScript OnUpdate, entity: {}", entity.GetId());
    }

    void TestScript::HandleEvent(const ECS::Entity& entity, const Ref<Event>&) {
        LA_INFO("TestScript HandleEvent");
    }
}
