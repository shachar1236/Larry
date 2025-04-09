#pragma once

#include "Layer.h"
#include "Renderer.h"
#include "TextureObject.h"

namespace Larry {
    class BackgroundLayer : public Layer {
        private:
            Renderer* renderer;
        public:
            BackgroundLayer();
            virtual ~BackgroundLayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(const Ref<Event>& event);
    };
}
