#pragma once

#include "gl.h"
#include "LarryApi.h"
#include "Layer/Layer.h"
#include "Renderer.h"

namespace Larry {
    class GUILayer : public Layer {
        private:
            Renderer* renderer;
        public:
            GUILayer(ECS::World* world_);
            virtual ~GUILayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(Event* event);
    };
}
