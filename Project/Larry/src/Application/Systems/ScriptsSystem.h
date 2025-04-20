#pragma once

#include "Event.h"
#include "Renderer.h"
#include "Systems/System.h"
#include "World.hpp"

namespace Larry {
    class ScriptsSystem : public ISystem {
        private:
        public:
            ScriptsSystem(const Ref<ECS::World>& world_) : ISystem(world_) {}
            ~ScriptsSystem() {}

            virtual void OnCreate();
            virtual void OnUpdate(double deltaTime);
            virtual void OnDelete();

            virtual void HandleEvent(const Ref<Event>& event);
    };
}
