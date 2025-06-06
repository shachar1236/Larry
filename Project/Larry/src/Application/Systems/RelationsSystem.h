#pragma once

#include "Components/Transform.h"
#include "Event.h"
#include "Systems/System.h"
#include "World.hpp"

namespace Larry {
    class RelationSystem : public ISystem {
        private:
            int counter = 0;
        public:
            RelationSystem(const Ref<ECS::World>& world_) : ISystem(world_) {}
            ~RelationSystem() {}

            virtual void OnCreate();
            virtual void OnUpdate(double deltaTime);
            virtual void OnDelete();

            Math::Vec3 GetParentTransform(ECS::Entity currEntity, Transform& currTransform);

            virtual void HandleEvent(const Ref<Event>& event);
    };
}
