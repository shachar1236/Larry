#pragma once

#include "EventSystem/Event.h"
#include "LarryMemory.h"

namespace Larry {
    class EventSystem {
        public:
            static void(*CallbackFunction)(const Ref<Event>&);
            static void SetCallbackFunction(void(callbackFunction)(const Ref<Event>&));

            static void DispatchEvent(const Ref<Event>& event);
    };


}
