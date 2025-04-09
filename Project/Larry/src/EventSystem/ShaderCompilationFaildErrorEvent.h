#pragma once
#include "ErrorEvent.h"
#include "Event.h"
#include <string>

namespace Larry {
    namespace Events {
        class ShaderCompilationFailedErrorEvent : public ErrorEvent {
            private:
                std::string infoLog;
                std::string shaderCode;
            public:
                ShaderCompilationFailedErrorEvent(const std::string& infoLog_, const std::string& shaderCode_) : ErrorEvent(""), infoLog(infoLog_), shaderCode(shaderCode_) {}
                virtual ~ShaderCompilationFailedErrorEvent () {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Error; }
                virtual enum EventType GetEventType() { return ShaderCompilationFailedErrorEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::OpenGLShaderCompilationFailedError; }

                virtual std::string GetErrorMessage() { return "Shader compilation failed: " + infoLog; }
                std::string GetInfoLog() { return infoLog; }
                std::string GetShaderCode() { return shaderCode; }
        };
    };
}
