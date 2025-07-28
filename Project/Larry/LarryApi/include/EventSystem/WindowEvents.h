#pragma once

#include "Event.h"
#include "GLFW/glfw3.h"
#include <string>

namespace Larry {
    namespace Events {
        class WindowResizedEvent : public Event {
            private:
                int width, height;
                GLFWwindow* window;
            public:
                WindowResizedEvent(const int& width_, const int& height_, GLFWwindow* window_) : width(width_), height(height_), window(window_) {};
                virtual ~WindowResizedEvent() {};

                virtual enum EventCategory GetEventCategory() { return EventCategory::Window; }
                virtual enum EventType GetEventType() { return WindowResizedEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::WindowResize; }

                int GetWidth() {
                    return width;
                }

                int GetHeight() {
                    return height;
                }

                GLFWwindow* GetWindow() {
                    return window;
                }
        };

        class WindowCloseEvent : public Event {
            private:
                GLFWwindow* window;
            public:
                WindowCloseEvent(GLFWwindow* window_) : window(window_) {};
                virtual ~WindowCloseEvent() {};

                virtual enum EventCategory GetEventCategory() { return EventCategory::Window; }
                virtual enum EventType GetEventType() { return WindowCloseEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::WindowClose; }

                GLFWwindow* GetWindow() {
                    return window;
                }
        };

        class WindowMoveEvent : public Event {
            private:
                GLFWwindow* window;
                int x, y;
            public:
                WindowMoveEvent(GLFWwindow* window_, const int& x_, const int& y_) : window(window_), x(x_), y(y_) {};
                virtual ~WindowMoveEvent() {};

                virtual enum EventCategory GetEventCategory() { return EventCategory::Window; }
                virtual enum EventType GetEventType() { return WindowMoveEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::WindowMoved; }

                GLFWwindow* GetWindow() {
                    return window;
                }
                int GetX() {
                    return x;
                }
                int GetY() {
                    return y;
                }
        };

        class WindowFocusEvent : public Event {
            private:
                GLFWwindow* window;
            public:
                WindowFocusEvent(GLFWwindow* window_) : window(window_) {};
                virtual ~WindowFocusEvent() {};

                virtual enum EventCategory GetEventCategory() { return EventCategory::Window; }
                virtual enum EventType GetEventType() { return WindowFocusEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::WindowFocus; }

                GLFWwindow* GetWindow() {
                    return window;
                }
        };


        class WindowLostFocusEvent : public Event {
            private:
                GLFWwindow* window;
            public:
                WindowLostFocusEvent(GLFWwindow* window_) : window(window_) {};
                virtual ~WindowLostFocusEvent() {};

                virtual enum EventCategory GetEventCategory() { return EventCategory::Window; }
                virtual enum EventType GetEventType() { return WindowLostFocusEvent::GetStaticType(); }
                static EventType GetStaticType() { return EventType::WindowLostFocus; }

                GLFWwindow* GetWindow() {
                    return window;
                }
        };
    };
}
