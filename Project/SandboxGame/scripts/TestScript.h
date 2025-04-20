#pragma once

#include "Scripts/Scripts.h"
#include "Utils/LarryMemory.h"

namespace Larry {
    class TestScript : public Scripts::Script {
        public:
            TestScript(const Ref<ECS::World>& world_);
            ~TestScript();
            
            virtual void OnCreate(const ECS::Entity& entity); // called when the entity is created
            virtual void OnUpdate(const ECS::Entity& entity, double deltaTime); // called every frame
            /* virtual void OnDelete(); // called when the entity is destroyed */

            virtual void HandleEvent(const ECS::Entity& entity, const Ref<Event>&);
    };
}
