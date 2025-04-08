#include "EventSystem.h"

namespace Larry {
    void(*EventSystem::CallbackFunction)(const Ref<Event>&);

    void EventSystem::SetCallbackFunction(void(callbackFunction)(const Ref<Event>&)) {
        EventSystem::CallbackFunction = callbackFunction;
    }

    void EventSystem::DispatchEvent(const Ref<Event>& event) {
        CallbackFunction(event);
    }
}
