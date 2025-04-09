#pragma once

#include "Layer.h"
#include "Renderer.h"
#include "TextureObject.h"

namespace Larry {
    class GUILayer : public Layer {
        private:
            Renderer* renderer;
        public:
            GUILayer();
            virtual ~GUILayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(const Ref<Event>& event);
    };
}
