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
                static EventType GetStaticType() { return EventType::WindowResized; }

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
    };
}
