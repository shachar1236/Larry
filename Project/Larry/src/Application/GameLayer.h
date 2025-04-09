#pragma once

#include "Layer.h"
#include "Math.h"
#include "Renderer.h"
#include "TextureObject.h"

namespace Larry {
    class GameLayer : public Layer {
        private:
            Renderer* renderer;

            Ref<TextureObject> face;
            Ref<TextureObject> test;
            Ref<TextureObject> wall;

            float speed = 10.0f;
            Math::Vec2 moveDirection = Math::Vec3(0.0f);
            Math::Vec2 position = Math::Vec2(200, 300);
        public:
            GameLayer();
            virtual ~GameLayer();

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(const Ref<Event>& event);

            void HandleKeyPressed(const Ref<Event>& event);
            void HandleKeyReleased(const Ref<Event>& event);
    };
}
