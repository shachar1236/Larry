#pragma once

#include "Event.h"
#include "Renderer.h"
#include "Systems/System.h"
#include "World.hpp"

namespace Larry {
    class ButtonSystem : public ISystem {
        private:
            Renderer* renderer;
            int mouseX, mouseY;
        public:
            ButtonSystem(const Ref<ECS::World>& world_) : ISystem(world_) {}
            ~ButtonSystem() {}

            virtual void OnCreate();
            virtual void OnUpdate(double deltaTime);
            virtual void OnDelete();
            void HandleMousePressedEvent(const Ref<Event>& event);
            virtual void HandleEvent(const Ref<Event>& event);
    };
}
