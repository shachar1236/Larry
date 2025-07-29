#pragma once

#include "EventSystem/Event.h"
#include "ECS/CPPApi/World.hpp"
#include "Utils/LarryMemory.h"

namespace Larry {
    class ISystem {
        protected:
            ECS::World* world;
        public:
            ISystem(ECS::World* world_) : world(world_) {}
            ~ISystem() {}

            virtual void OnCreate() = 0;
            virtual void OnUpdate(double deltaTime) = 0;
            virtual void OnDelete() = 0;

            virtual void HandleEvent(Event* event) = 0;
    };
}
