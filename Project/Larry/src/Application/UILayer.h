#pragma once

#include "Layer.h"
#include "Renderer.h"
#include "TextureObject.h"

namespace Larry {
    class UILayer : public Layer {
        private:
            Renderer* renderer;
        public:
            UILayer();
            virtual ~UILayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(const Ref<Event>& event);
    };
}
