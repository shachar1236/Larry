#pragma once
#include "Event.h"
#include <string>

namespace Larry {
    namespace Events {
        class ErrorEvent : public Event {
            private:
                std::string errorMessage;
                bool fatal;
            public:
                ErrorEvent(const std::string& errorMessage_, const bool& fatal_) : errorMessage(errorMessage_), fatal(fatal_) {}
                virtual ~ErrorEvent() {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Error; }
                virtual enum EventType GetEventType() { return ErrorEvent::GetEventType(); }
                static EventType GetStaticType() { return EventType::RegularError; }

                virtual std::string GetErrorMessage() { return errorMessage; }

                bool IsFatal() { return fatal; }
        };

        class ShaderCompilationFailedErrorEvent : public ErrorEvent {
            private:
                std::string infoLog;
                std::string shaderCode;
            public:
                ShaderCompilationFailedErrorEvent(const std::string& infoLog_, const std::string& shaderCode_) : ErrorEvent("", true), infoLog(infoLog_), shaderCode(shaderCode_) {}
                virtual ~ShaderCompilationFailedErrorEvent () {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Error; }
                virtual enum EventType GetEventType() { return ShaderCompilationFailedErrorEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::OpenGLShaderCompilationFailedError; }

                virtual std::string GetErrorMessage() { return "Shader compilation failed: " + infoLog; }
                std::string GetInfoLog() { return infoLog; }
                std::string GetShaderCode() { return shaderCode; }
        };

        class GlfwErrorEvent : public ErrorEvent {
            private:
                int errorCode;
                std::string error;
            public:
                GlfwErrorEvent (const std::string& error_, const int& errorCode_) : ErrorEvent("", true), error(error_), errorCode(errorCode_) {}
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
