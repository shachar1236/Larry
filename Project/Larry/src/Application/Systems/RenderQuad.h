#pragma once

#include "EventSystem/Event.h"
#include "Renderer.h"
#include "Systems/System.h"
#include "ECS/CPPApi/World.hpp"

namespace Larry {
    class RenderQuad : public ISystem {
        private:
            Renderer* renderer;
        public:
            RenderQuad(const Ref<ECS::World>& world_) : ISystem(world_) {}
            ~RenderQuad() {}

            virtual void OnCreate();
            virtual void OnUpdate(double deltaTime);
            virtual void OnDelete();

            virtual void HandleEvent(const Ref<Event>& event);
    };
}
