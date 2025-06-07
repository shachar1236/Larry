#pragma once
#include "ECS/CPPApi/World.hpp"
#include "ECS/ECS_C.h"
#include "EventSystem/Event.h"
#include "EventSystem/InputEvents.h"
#include "Layers/LayerStack.h"
#include "Utils/LarryMemory.h"
#include <functional>
#include <string>
#include <vector>

// #define SCRIPT_TYPE(T) static ECS_TypeHashCode GetType() { return typeid(T).hash_code(); }
// #define SCRIPT_LAYER(name) static char* GetScriptLayer() { return #name; }


#define DEFINE_SCRIPT(T, layer) \
    Ref<Larry::Scripts::Script> CreateScript##T(const Ref<Larry::ECS::World>& world) \
    { \
        return Larry::CreateRef<T>(world); \
    } \
    struct AutoCall##T \
    { \
        AutoCall##T() { \
            Larry::Scripts::Script::scriptType_to_layerName[typeid(T).hash_code()] = layer; \
            Larry::Scripts::Script::scriptName_to_CreateFunction[#T] = CreateScript##T; \
        } \
    }; \
    AutoCall##T autoCallObject##T;

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
            Script(const Ref<ECS::World> world_)
                : world(world_) { };
            virtual ~Script() { };

            virtual void OnCreate(const ECS::Entity& entity) = 0; // called when the entity is created
            virtual void OnUpdate(const ECS::Entity& entity, double deltaTime) = 0; // called every frame
            /* virtual void OnDelete() = 0; // called when the entity is destroyed */

            virtual void ButtonJustPressed(const ECS::Entity&, Events::KeyPressedEvent*) { }; // Called when a button was first pressed
            virtual void ButtonReleased(const ECS::Entity&, Events::KeyReleasedEvent*) { }; // Called when a button was released
            virtual void MouseJustPressed(const ECS::Entity&, Events::MousePressedEvent*) { }; // Called when one of the mouse first  buttons was pressed
            virtual void MouseScrolled(const ECS::Entity&, Events::MouseScrolledEvent*) { }; // Called when one of the mouse was scrolled

            virtual void HandleEvent(const ECS::Entity& entity, const Ref<Event>&) = 0;

            // static ECS_TypeHashCode GetType() { return 0; };
            // static char* GetScriptLayer() { return nullptr; };

            using CreateScriptFunction = Ref<Script>(*)(const Ref<ECS::World>&);

            static std::unordered_map<ECS_TypeHashCode, char*> scriptType_to_layerName;
            static std::unordered_map<std::string, CreateScriptFunction> scriptName_to_CreateFunction;

            static void Init(const Ref<ECS::World>& world);
            static void RegisterScripts(LayerStack&);

            static Ref<Script> GetNewInstanceOfScript(const std::string& name, const Ref<ECS::World>& world) {
                return (scriptName_to_CreateFunction[name])(world);
            }
        };

    using ScriptsComponent = std::vector<Ref<Script>>;
}
