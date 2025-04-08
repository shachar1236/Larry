#pragma once
#include "Event.h"
#include <string>

namespace Larry {
    namespace Events {
        class ErrorEvent : public Event {
            private:
                std::string errorMessage;
            public:
                ErrorEvent(const std::string& errorMessage_);
                virtual ~ErrorEvent();

                virtual enum EventCategory GetEventCategory() { return EventCategory::Error; }
                virtual enum EventType GetEventType() { return EventType::RegularError; }

                virtual std::string GetErrorMessage();
        };
    };
}
