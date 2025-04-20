#pragma once

#include "Scripts/Scripts.h"
#include "Utils/LarryMemory.h"
#include <string>

namespace Larry::Scripts {
    class TestScript : public Script {
        public:
            TestScript();
            ~TestScript();
            
            virtual void OnCreate(); // called when the entity is created
            virtual void OnUpdate(double deltaTime); // called every frame
            /* virtual void OnDelete(); // called when the entity is destroyed */

            virtual void HandleEvent(const Ref<Event>&);
    };
}
