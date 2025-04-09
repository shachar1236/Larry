#pragma once

#include "Layer.h"
#include "Renderer.h"
#include "TextureObject.h"

namespace Larry {
    class GameLayer : public Layer {
        private:
            Renderer* renderer;

            Ref<TextureObject> face;
            Ref<TextureObject> test;
            Ref<TextureObject> wall;
        public:
            GameLayer();
            virtual ~GameLayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(const Ref<Event>& event);
    };
}
