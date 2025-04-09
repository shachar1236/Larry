#pragma once
#include "ErrorEvent.h"
#include "Event.h"
#include <string>

namespace Larry {
    namespace Events {
        class GlfwErrorEvent : public ErrorEvent {
            private:
                int errorCode;
                std::string error;
            public:
                GlfwErrorEvent (const std::string& error_, const int& errorCode_) : ErrorEvent(""), error(error_), errorCode(errorCode_) {}
                virtual ~GlfwErrorEvent  () {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Error; }
                virtual enum EventType GetEventType() { return GlfwErrorEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::GlfwError; }

                virtual std::string GetErrorMessage() { 
                    std::string ret;
                    ret += "Error code ";
                    ret += std::to_string(errorCode);
                    ret += std::string(" error: ") + error;
                    return ret;
                }
                virtual std::string GetErrorCode() { return error; }
        };
    };
}
