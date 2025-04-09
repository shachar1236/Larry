#include "LarryWindow.h"
#include "ErrorEvents.h"
#include "GLFW/glfw3.h"
#include "LarryMemory.h"
#include "EventSystem.h"
#include "Log.h"
#include "WindowEvents.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Larry::Ref<Larry::Events::WindowResizedEvent> event = Larry::CreateRef<Larry::Events::WindowResizedEvent>(width, height, window);
    Larry::EventSystem::HandleEvent(event);
}

void window_close_callback(GLFWwindow* window)
{
    Larry::Ref<Larry::Events::WindowCloseEvent> event = Larry::CreateRef<Larry::Events::WindowCloseEvent>(window);
    Larry::EventSystem::HandleEvent(event);
}

void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    Larry::Ref<Larry::Events::WindowMoveEvent> event = Larry::CreateRef<Larry::Events::WindowMoveEvent>(window, xpos, ypos);
    Larry::EventSystem::HandleEvent(event);
}

void window_focus_callback(GLFWwindow* window, int focused)
{
    if (focused)
    {
        Larry::Ref<Larry::Events::WindowFocusEvent> event = Larry::CreateRef<Larry::Events::WindowFocusEvent>(window);
        Larry::EventSystem::HandleEvent(event);
    }
    else
    {
        Larry::Ref<Larry::Events::WindowLostFocusEvent> event = Larry::CreateRef<Larry::Events::WindowLostFocusEvent>(window);
        Larry::EventSystem::HandleEvent(event);
    }
}

namespace Larry {

    LarryWindow::LarryWindow(const WindowConfig& config_) : config(config_) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (config.maximized) {
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        }
        GLFWmonitor* monitor = NULL;
        if (config.fullscreen) {
            monitor = glfwGetPrimaryMonitor();
        }
        window = glfwCreateWindow(config.window_width, config.window_height, config.title.c_str(), monitor, NULL);

        if (window == NULL)
        {
            Ref<Events::ErrorEvent> event = CreateRef<Events::ErrorEvent>("Failed to create GLFW window", true);
            EventSystem::HandleEvent(event);
            glfwTerminate();
        }

        glfwMakeContextCurrent(window);

        // set callbacks
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
        glfwSetWindowCloseCallback(window, window_close_callback);
        glfwSetWindowFocusCallback(window, window_focus_callback);
    }

    LarryWindow::~LarryWindow() {

    }

    bool LarryWindow::ShouldClose() {
        return glfwWindowShouldClose(window);
    }

    void LarryWindow::SetFulllscreen(int xpos, int ypos, int width, int height, int refreshRate) {
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), xpos, ypos, width, height, refreshRate);
    }

    void LarryWindow::SetWindowed(int xpos, int ypos, int width, int height, int refreshRate) {
        glfwSetWindowMonitor(window, NULL, xpos, ypos, width, height, refreshRate);
    }

    void LarryWindow::UpdateResulotion(int width, int height) {
        glfwSetWindowSize(window, width, height);
    }

    void LarryWindow::SetWindowSizeLimits(int minwidth, int minheight, int maxwidth, int maxheight) {
        glfwSetWindowSizeLimits(window, minwidth, minheight, maxwidth, maxheight);
    }

    void LarryWindow::SetWindowIcon(GLFWimage* images, int images_number) {
        glfwSetWindowIcon(window, images_number, images);
    }

    void LarryWindow::Maximize() {
         glfwMaximizeWindow(window);
    }

    void LarryWindow::Unmaximize() {
        glfwRestoreWindow(window);
    }

    void LarryWindow::SetCursorMode(CursorMode mode) {
        glfwSetInputMode(window, GLFW_CURSOR, mode);
    }
}
