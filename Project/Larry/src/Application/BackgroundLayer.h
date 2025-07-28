#pragma once

#include "Layer/Layer.h"
#include "Math/Math.h"
#include "Renderer.h"
#include "ECS/Internal/World.hpp"

namespace Larry {
    class BackgroundLayer : public Layer {
        private:
            Math::Vec4 color;
            double width, height;
        public:
            BackgroundLayer(const Ref<ECS::World>& world_);
            virtual ~BackgroundLayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(const Ref<Event>& event);
    };
}
