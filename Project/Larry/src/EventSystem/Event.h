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
        RegularError,
        OpenGLError,
        OpenGLShaderCompilationFailedError,

        WindowResized,
    };
    inline auto format_as(enum EventType f) { return fmt::underlying(f); }

    class Event {
        public:
            bool Handeled;
            Event() {};

            virtual EventCategory GetEventCategory() = 0;
            virtual EventType GetEventType() = 0;
    };
}
