#pragma once

#include "ECS/Entity.hpp"
#include "EventSystem/Event.h"
#include "Utils/LarryMemory.h"
#include "ECS/World.hpp"
#include <string>
#include <vector>

// GameEngine API for scripts
// Math
// ECS
// Components
// Utils
// TextureLoading
// Events
// Input
namespace Larry::Scripts {
    class Script {
        protected:
            Ref<ECS::World> world;
        public:
            Script(const Ref<ECS::World> world_) : world(world_) {};
            virtual ~Script() {};
            
            virtual void OnCreate(const ECS::Entity& entity) = 0; // called when the entity is created
            virtual void OnUpdate(const ECS::Entity& entity, double deltaTime) = 0; // called every frame
            /* virtual void OnDelete() = 0; // called when the entity is destroyed */

            virtual void HandleEvent(const ECS::Entity& entity, const Ref<Event>&) = 0;

        static void Init();
        static Ref<Script> GetNewInstanceOfScript(const std::string& name, const Ref<ECS::World>& world);
    };

    using ScriptsComponent = std::vector<Ref<Script>>;
}
