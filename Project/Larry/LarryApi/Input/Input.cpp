#include "common.h"
#include "gl.h"
#include "Input.h"
#include "EventSystem/InputEvents.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventSystem.h"
#include "LarryMemory.h"
#include "Math.h"
#include "EventSystem/WindowEvents.h"
#include <mutex>

namespace Larry {
    std::mutex width_height_mtx;
    int width, height;

    std::mutex mouse_mtx;
    Math::Vec2 mousePos;

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        switch (action) {
            case GLFW_PRESS:
                {
                    Events::KeyPressedEvent* event = new Events::KeyPressedEvent(window, key);
                    EventSystem::HandleEvent(event);
                }
                break;
            case GLFW_RELEASE:
                {
                    Events::KeyReleasedEvent* event = new Events::KeyReleasedEvent(window, key);
                    EventSystem::HandleEvent(event);
                }
                break;
            default:
                break;
        }
    }

    void character_callback(GLFWwindow* window, unsigned int codepoint)
    {
        Events::KeyTypedEvent* event = new Events::KeyTypedEvent(window, codepoint);
        EventSystem::HandleEvent(event);
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        std::lock_guard guard(width_height_mtx);
        std::lock_guard guard2(mouse_mtx);
        mousePos.x = xpos;
        mousePos.y = height - ypos;

        Events::MouseMovedEvent* event = new Events::MouseMovedEvent(window, xpos, height - ypos);
        EventSystem::HandleEvent(event);
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        switch (action) {
            case GLFW_PRESS:
                {
                    Events::MousePressedEvent* event = new Events::MousePressedEvent(window, button);
                    EventSystem::HandleEvent(event);
                }
                break;
            case GLFW_RELEASE:
                {
                    Events::MouseReleasedEvent* event = new Events::MouseReleasedEvent(window, button);
                    EventSystem::HandleEvent(event);
                }
                break;
            default:
                break;
        }
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Events::MouseScrolledEvent* event = new Events::MouseScrolledEvent(window, xoffset, yoffset);
        EventSystem::HandleEvent(event);
    }

    void* Input::void_window = nullptr;

    void Input::Init(void* window_) {
        void_window = window_;
        GLFWwindow* window = (GLFWwindow*)void_window;
        glfwSetKeyCallback(window, key_callback);
        glfwSetCharCallback(window, character_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window, scroll_callback);

        EventSystem::AddCallbackFunction(HandleEvent);
    };

    void Input::HandleEvent(Event* event) {
        DispatchEvent<Events::WindowResizedEvent>(event, [&](Events::WindowResizedEvent* window_event){
            std::lock_guard guard(width_height_mtx);
            width = window_event->GetWidth();
            height = window_event->GetHeight();
        });
    }


    bool Input::KeyPressed(int key) {
        return glfwGetKey((GLFWwindow*)void_window, key) == GLFW_PRESS;
    }

    int Input::MouseX() {
        return mousePos.x;
    }

    int Input::MouseY() {
        return mousePos.y;
    }

    Math::Vec2 Input::Mouse() {
        return mousePos;
    }
}
