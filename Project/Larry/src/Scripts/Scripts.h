#pragma once
#include "ECS/Entity.hpp"
#include "EventSystem/Event.h"
#include "Utils/LarryMemory.h"
#include "ECS/World.hpp"
#include "EventSystem/InputEvents.h"
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

            virtual void ButtonJustPressed(const ECS::Entity&, Events::KeyPressedEvent*) {}; // Called when a button was first pressed
            virtual void ButtonReleased(const ECS::Entity&, Events::KeyReleasedEvent*) {}; // Called when a button was released
            virtual void MouseJustPressed(const ECS::Entity&, Events::MousePressedEvent*) {}; // Called when one of the mouse first  buttons was pressed
            virtual void MouseScrolled(const ECS::Entity&, Events::MouseScrolledEvent*) {}; // Called when one of the mouse was scrolled
                                                               //
            virtual void HandleEvent(const ECS::Entity& entity, const Ref<Event>&) = 0;

        static void Init(const Ref<ECS::World>& world);
        static Ref<Script> GetNewInstanceOfScript(const std::string& name, const Ref<ECS::World>& world);
    };

    using ScriptsComponent = std::vector<Ref<Script>>;
}
