#pragma once

#include "spdlog/fmt/bundled/format.h"

namespace Larry {
    enum EventCategory {
        Error,
        Input,
        Window,
    };
    inline auto format_as(enum EventCategory f) { return fmt::underlying(f); }

    enum EventType {
        None = 0,
        RegularError,
        OpenGLError,
        OpenGLShaderCompilationFailedError,

        WindowResized,
    };
    inline auto format_as(enum EventType f) { return fmt::underlying(f); }

    class Event {
        public:
            bool Handeled; // set this to true if you want the event to not be handled any more
            Event() {};

            virtual EventCategory GetEventCategory() = 0;
            virtual EventType GetEventType() = 0;

            static EventType GetStaticType() { return EventType::None; }
    };
}
