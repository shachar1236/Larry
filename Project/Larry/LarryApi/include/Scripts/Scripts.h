#pragma once
#include "ECS/CPPApi/World.hpp"
#include "ECS/ECS_C.h"
#include "EventSystem/Event.h"
#include "EventSystem/InputEvents.h"
#include "Utils/LarryMemory.h"
#include <string>

// #define SCRIPT_TYPE(T) static ECS_TypeHashCode GetType() { return typeid(T).hash_code(); }
// #define SCRIPT_LAYER(name) static char* GetScriptLayer() { return #name; }

#define DEFINE_SCRIPT(T, layer) \
    void CreateScript##T(void* ptr, const Ref<Larry::ECS::World>& world) \
    { \
        T* a = (T*)ptr; \
        new (a) T(world); \
    } \
    void DestractorForScript##T(const void* x) \
    { \
        static_cast<const T*>(x)->~T(); \
    } \
    struct AutoCall##T \
    { \
        AutoCall##T() { \
            Larry::Scripts::Script::scriptName_to_detailes[#T] = {}; \
            Larry::Scripts::Script::scriptName_to_detailes[#T].hash_code = typeid(T).hash_code(); \
            Larry::Scripts::Script::scriptName_to_detailes[#T].size = sizeof(T); \
            Larry::Scripts::Script::scriptName_to_detailes[#T].create_function = CreateScript##T; \
            Larry::Scripts::Script::scriptName_to_detailes[#T].destractor_function = DestractorForScript##T; \
            Larry::Scripts::Script::scriptName_to_detailes[#T].layer_name = layer; \
        } \
    }; \
    AutoCall##T autoCallObject##T;

// GameEngine API for scripts
// Math
// ECS
// Components
// Utils
// TextureLoading
// Input
// Events
namespace Larry::Scripts {
    class Script {
        protected:
            ECS::World* world;

        public:
            Script(ECS::World* world_)
                : world(world_) { };
            virtual ~Script() { };

            virtual void OnCreate(const ECS::Entity& entity) = 0; // called when the entity is created
            virtual void OnUpdate(const ECS::Entity& entity, double deltaTime) = 0; // called every frame
            /* virtual void OnDelete() = 0; // called when the entity is destroyed */

            virtual void ButtonJustPressed(const ECS::Entity&, Events::KeyPressedEvent*) { }; // Called when a button was first pressed
            virtual void ButtonReleased(const ECS::Entity&, Events::KeyReleasedEvent*) { }; // Called when a button was released
            virtual void MouseJustPressed(const ECS::Entity&, Events::MousePressedEvent*) { }; // Called when one of the mouse first  buttons was pressed
            virtual void MouseScrolled(const ECS::Entity&, Events::MouseScrolledEvent*) { }; // Called when one of the mouse was scrolled

            virtual void HandleEvent(const ECS::Entity& entity, Event*) = 0;

            // static ECS_TypeHashCode GetType() { return 0; };
            // static char* GetScriptLayer() { return nullptr; };

            using CreateScriptFunction = void(*)(void*, ECS::World*);
            using DestructorScriptFunction = void(*)(const void*);

            struct scriptDetails {
                ECS_TypeHashCode hash_code;
                int size;
                CreateScriptFunction create_function;
                DestructorScriptFunction destractor_function;
                char* layer_name;
            };
            static std::unordered_map<std::string, scriptDetails> scriptName_to_detailes;

            static void Init(ECS::World* world);
        };
}
