#pragma once
#include "EventSystem/Event.h"

namespace Larry {
    namespace Events {
        class SystemInitEvent : public Event {
            public:
                SystemInitEvent()  {}
                virtual ~SystemInitEvent() {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::System; }
                virtual enum EventType GetEventType() { return SystemInitEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::SystemInit; }
        };
    }
}
