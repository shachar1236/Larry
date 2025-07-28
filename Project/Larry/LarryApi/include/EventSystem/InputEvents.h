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

        class MouseMovedEvent : public Event {
            private:
                GLFWwindow* window;
                double x, y;
            public:
                MouseMovedEvent(GLFWwindow* window_, const double& x_, const double& y_) : window(window_), x(x_), y(y_) {}
                virtual ~MouseMovedEvent() {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Input; }
                virtual enum EventType GetEventType() { return MouseMovedEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::MouseMoved; }

                GLFWwindow* GetWindow() {
                    return window;
                }

                double GetX() {
                    return x;
                }

                double GetY() {
                    return y;
                }
        };

        class MousePressedEvent : public Event {
            private:
                GLFWwindow* window;
                int button;
            public:
                MousePressedEvent(GLFWwindow* window_, const int& button_) : window(window_), button(button_) {}
                virtual ~MousePressedEvent() {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Input; }
                virtual enum EventType GetEventType() { return MousePressedEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::MouseButtonPressed; }

                GLFWwindow* GetWindow() {
                    return window;
                }

                int GetButton() {
                    return button;
                }
        };


        class MouseReleasedEvent : public Event {
            private:
                GLFWwindow* window;
                int button;
            public:
                MouseReleasedEvent(GLFWwindow* window_, const int& button_) : window(window_), button(button_) {}
                virtual ~MouseReleasedEvent() {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Input; }
                virtual enum EventType GetEventType() { return MouseReleasedEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::MouseButtonReleased; }

                GLFWwindow* GetWindow() {
                    return window;
                }

                int GetButton() {
                    return button;
                }
        };

        class MouseScrolledEvent : public Event {
            private:
                GLFWwindow* window;
                double xoffset, yoffset;
            public:
                MouseScrolledEvent(GLFWwindow* window_, const double& x, const double& y) : window(window_), xoffset(x), yoffset(y) {}
                virtual ~MouseScrolledEvent() {}

                virtual enum EventCategory GetEventCategory() { return EventCategory::Input; }
                virtual enum EventType GetEventType() { return MouseScrolledEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::MouseScrolled; }

                GLFWwindow* GetWindow() {
                    return window;
                }

                double GetXOffset() {
                    return xoffset;
                }

                double GetYOffset() {
                    return yoffset;
                }
        };
    }
}
