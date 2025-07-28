#pragma once

#include "EventSystem/Event.h"
#include "Math/Math.h"
#include "Utils/LarryMemory.h"

namespace Larry {
    class Input {
        private:
            static void* void_window;
        public:
            static void HandleEvent(const Ref<Event>& event);

            static void Init(void* window_);
            Input() {};
            ~Input() {};

            static bool KeyPressed(int key);

            static int MouseX();
            static int MouseY();
            static Math::Vec2 Mouse();
    };
}
