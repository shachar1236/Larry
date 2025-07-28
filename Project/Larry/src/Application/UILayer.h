#pragma once

#include "LarryApi.h"
#include "Layer/Layer.h"
#include "ECS/Internal/Entity.hpp"
#include "Renderer.h"

namespace Larry {
    class UILayer : public Layer {
        private:
            ECS::Entity button;
        public:
            UILayer(const Ref<ECS::World>& world_);
            virtual ~UILayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(const Ref<Event>& event);
    };
}
