#pragma once

#include "LarryApi.h"
#include "Renderer.h"
#include "Systems/System.h"
#include "ECS/CPPApi/World.hpp"

namespace Larry {
    class ButtonSystem : public ISystem {
        private:
            Renderer* renderer;
            int mouseX, mouseY;
        public:
            ButtonSystem(ECS::World* world_) : ISystem(world_) {}
            ~ButtonSystem() {}

            virtual void OnCreate();
            virtual void OnUpdate(double deltaTime);
            virtual void OnDelete();
            void HandleMousePressedEvent(Events::MousePressedEvent* event);
            virtual void HandleEvent(Event* event);
    };
}
