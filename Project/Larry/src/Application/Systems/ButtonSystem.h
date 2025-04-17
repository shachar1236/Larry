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
            ButtonSystem() {}
            ~ButtonSystem() {}

            virtual void OnCreate(ECS::World&);
            virtual void OnUpdate(ECS::World&, const double& deltaTime);
            virtual void OnDelete(ECS::World&);
            void HandleMousePressedEvent(const Ref<Event>& event, ECS::World& world);
            virtual void HandleEvent(ECS::World&, const Ref<Event>& event);
    };
}
