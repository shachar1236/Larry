#pragma once

#include "Event.h"
#include "World.hpp"

namespace Larry {
    class ISystem {
        public:
            ISystem() {}
            ~ISystem() {}

            virtual void OnCreate(ECS::World&) = 0;
            virtual void OnUpdate(ECS::World&, const double& deltaTime) = 0;
            virtual void OnDelete(ECS::World&) = 0;

            virtual void HandleEvent(ECS::World&, const Ref<Event>& event) = 0;
    };
}
