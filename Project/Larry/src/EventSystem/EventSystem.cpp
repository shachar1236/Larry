#include "EventSystem.h"

namespace Larry {
    using EventCallbackFunction = std::function<void(const Ref<Event>&)> ;
    std::vector<EventCallbackFunction> EventSystem::CallbackFunctions;
    std::queue<Ref<Event>> EventSystem::event_queue;

    void EventSystem::AddCallbackFunction(const EventCallbackFunction& cl) {
        EventSystem::CallbackFunctions.push_back(cl);
    }

    void EventSystem::HandleEvent(const Ref<Event>& event) {
        event_queue.push(event);
    }

    void EventSystem::HandleQueuedEvents() {
        while (!event_queue.empty()) {
            Ref<Event> event = event_queue.front();
            event_queue.pop();
            for (int i = 0; i < EventSystem::CallbackFunctions.size(); i++) {
                EventSystem::CallbackFunctions[i](event);
            }
        }
    }

}
