#pragma once

#include "Event.h"
#include "Renderer.h"
#include "Systems/System.h"
#include "World.hpp"

namespace Larry {
    class RenderQuad : public ISystem {
        private:
            Renderer* renderer;
        public:
            RenderQuad() {}
            ~RenderQuad() {}

            virtual void OnCreate(ECS::World&);
            virtual void OnUpdate(ECS::World&, const double& deltaTime);
            virtual void OnDelete(ECS::World&);

            virtual void HandleEvent(ECS::World&, const Ref<Event>& event);
    };
}
