#pragma once

#include "gl.h"
#include "Layer/Layer.h"
#include "ECS/Internal/Entity.hpp"
#include "LarryApi.h"
#include "Renderer.h"

namespace Larry {
    class GameLayer : public Layer {
        private:
        public:
            GameLayer(const Ref<ECS::World>& world_);
            virtual ~GameLayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(const Ref<Event>& event);

            void HandleKeyPressed(const Ref<Event>& event);
            void HandleKeyReleased(const Ref<Event>& event);

            void HandleMouseMoved(const Ref<Event>& event);
            void HandleMousePressed(const Ref<Event>& event);
            void HandleMouseReleased(const Ref<Event>& event);
    };
}
