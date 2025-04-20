#pragma once

#include "Event.h"
#include "World.hpp"

namespace Larry {
    class ISystem {
        protected:
            Ref<ECS::World> world;
        public:
            ISystem(const Ref<ECS::World>& world_) : world(world_) {}
            ~ISystem() {}

            virtual void OnCreate() = 0;
            virtual void OnUpdate(double deltaTime) = 0;
            virtual void OnDelete() = 0;

            virtual void HandleEvent(const Ref<Event>& event) = 0;
    };
}
