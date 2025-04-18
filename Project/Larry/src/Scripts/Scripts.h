#pragma once

#include "Utils/LarryMemory.h"
#include <string>

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
        public:
            Script() {};
            ~Script() {};
            
            virtual void OnCreate() = 0; // called when the entity is created
            virtual void OnUpdate(double deltaTime) = 0; // called every frame
            virtual void OnDelete() = 0; // called when the entity is destroyed

        static void Init();
        static Ref<Script> GetNewInstanceOfScript(const std::string& name);
    };
}
