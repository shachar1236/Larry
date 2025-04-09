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

    void InitInput(GLFWwindow* window) {
        glfwSetKeyCallback(window, key_callback);
        glfwSetCharCallback(window, character_callback);
    };
}
