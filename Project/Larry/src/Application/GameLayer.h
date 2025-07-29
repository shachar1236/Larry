#pragma once

#include "gl.h"
#include "EventSystem/InputEvents.h"
#include "Layer/Layer.h"
#include "ECS/Internal/Entity.hpp"
#include "LarryApi.h"
#include "Renderer.h"

namespace Larry {
    class GameLayer : public Layer {
        private:
        public:
            GameLayer(ECS::World* world_);
            virtual ~GameLayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(Event* event);

            void HandleKeyPressed(Events::KeyPressedEvent* event);
            void HandleKeyReleased(Events::KeyReleasedEvent* event);

            void HandleMouseMoved(Events::MouseMovedEvent* event);
            void HandleMousePressed(Events::MousePressedEvent* event);
            void HandleMouseReleased(Events::MouseReleasedEvent* event);
    };
}
