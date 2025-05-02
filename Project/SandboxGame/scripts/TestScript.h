#pragma once

#include "LarryApi.h"

namespace Larry {
    class TestScript : public Scripts::Script {
        private:
            TextureLoader* texture_loader;
            Ref<TextureObject> face_texture;
            float vel = 400;
            float camera_vel = 100;
            Math::Vec3 cameraPos;
            int gameLayerId;
        public:
            TestScript(const Ref<ECS::World>& world_);
            ~TestScript();
            
            virtual void OnCreate(const ECS::Entity& entity); // called when the entity is created
            virtual void OnUpdate(const ECS::Entity& entity, double deltaTime); // called every frame
            /* virtual void OnDelete(); // called when the entity is destroyed */

            virtual void HandleEvent(const ECS::Entity& entity, const Ref<Event>&);
    };
}
