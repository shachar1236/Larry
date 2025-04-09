#include "Input.h"
#include "InputEvents.h"
#include "Event.h"
#include "EventSystem.h"
#include "GLFW/glfw3.h"
#include "LarryMemory.h"

namespace Larry {
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        switch (action) {
            case GLFW_PRESS:
                {
                    Ref<Events::KeyPressedEvent> event = CreateRef<Events::KeyPressedEvent>(window, key);
                    EventSystem::HandleEvent(event);
                }
                break;
            case GLFW_RELEASE:
                {
                    Ref<Events::KeyReleasedEvent> event = CreateRef<Events::KeyReleasedEvent>(window, key);
                    EventSystem::HandleEvent(event);
                }
                break;
            default:
                break;
        }
    }

    void character_callback(GLFWwindow* window, unsigned int codepoint)
    {
        Ref<Events::KeyTypedEvent> event = CreateRef<Events::KeyTypedEvent>(window, codepoint);
        EventSystem::HandleEvent(event);
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        Ref<Events::MouseMovedEvent> event = CreateRef<Events::MouseMovedEvent>(window, xpos, ypos);
        EventSystem::HandleEvent(event);
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        switch (action) {
            case GLFW_PRESS:
                {
                    Ref<Events::MousePressedEvent> event = CreateRef<Events::MousePressedEvent>(window, button);
                    EventSystem::HandleEvent(event);
                }
                break;
            case GLFW_RELEASE:
                {
                    Ref<Events::MouseReleasedEvent> event = CreateRef<Events::MouseReleasedEvent>(window, button);
                    EventSystem::HandleEvent(event);
                }
                break;
            default:
                break;
        }
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Ref<Events::MouseScrolledEvent> event = CreateRef<Events::MouseScrolledEvent>(window, xoffset, yoffset);
        EventSystem::HandleEvent(event);
    }

    void InitInput(GLFWwindow* window) {
        glfwSetKeyCallback(window, key_callback);
        glfwSetCharCallback(window, character_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window, scroll_callback);
    };
}
