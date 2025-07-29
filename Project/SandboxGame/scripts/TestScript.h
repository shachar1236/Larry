#pragma once

#include "LarryApi.h"
#include "Components/Transform.h"

namespace Larry {
    class TestScript : public Scripts::Script {
        private:
            TextureLoader* texture_loader;
            TextureObject* face_texture;
            float vel = 400;
            int gameLayerId;

            ECS::ComponentPtr<Transform> transform;
        public:
            TestScript(ECS::World* world_);
            ~TestScript();
            
            virtual void OnCreate(const ECS::Entity& entity); // called when the entity is created
            virtual void OnUpdate(const ECS::Entity& entity, double deltaTime); // called every frame
            /* virtual void OnDelete(); // called when the entity is destroyed */

            virtual void HandleEvent(const ECS::Entity& entity, Event*);
    };
}
