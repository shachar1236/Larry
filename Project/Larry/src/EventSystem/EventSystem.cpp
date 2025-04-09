#include "EventSystem.h"

namespace Larry {
    using EventCallbackFunction = std::function<void(const Ref<Event>&)> ;
    EventCallbackFunction EventSystem::CallbackFunction;

    void EventSystem::SetCallbackFunction(const EventCallbackFunction& cl) {
        EventSystem::CallbackFunction = cl;
    }

    void EventSystem::HandleEvent(const Ref<Event>& event) {
        CallbackFunction(event);
    }

}
