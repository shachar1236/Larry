#pragma once
#include "Event.h"
#include "GLFW/glfw3.h"

namespace Larry {
    namespace Events {
        class KeyPressedEvent : public Event {
            private:
                GLFWwindow* window;
                int key;
            public:
                KeyPressedEvent(GLFWwindow* window_, const int& key_) : window(window_), key(key_) {}
                virtual ~KeyPressedEvent() {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Input; }
                virtual enum EventType GetEventType() { return KeyPressedEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::KeyPressed; }

                GLFWwindow* GetWindow() {
                    return window;
                }

                int GetKey() {
                    return key;
                }
        };

        class KeyReleasedEvent : public Event {
            private:
                GLFWwindow* window;
                int key;
            public:
                KeyReleasedEvent(GLFWwindow* window_, const int& key_) : window(window_), key(key_) {}
                virtual ~KeyReleasedEvent() {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Input; }
                virtual enum EventType GetEventType() { return KeyReleasedEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::KeyReleased; }

                GLFWwindow* GetWindow() {
                    return window;
                }

                int GetKey() {
                    return key;
                }
        };

        class KeyTypedEvent : public Event {
            private:
                GLFWwindow* window;
                unsigned int codepoint;
            public:
                KeyTypedEvent(GLFWwindow* window_, const unsigned int& codepoint_) : window(window_), codepoint(codepoint_) {}
                virtual ~KeyTypedEvent() {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Input; }
                virtual enum EventType GetEventType() { return KeyTypedEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::KeyTyped; }

                GLFWwindow* GetWindow() {
                    return window;
                }

                unsigned int GetCodepoint() {
                    return codepoint;
                }
        };
    }
}
