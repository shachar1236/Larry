#include "Application.h"
#include "ErrorEvents.h"
#include "Event.h"
#include "EventSystem/EventSystem.h"
#include "GLFW/glfw3.h"
#include "Input.h"
#include "LarryMemory.h"
#include "LarryWindow.h"
#include "Log.h"
#include "LayerStack.h"
#include "BackgroundLayer.h"
#include "GameLayer.h"
#include "UILayer.h"
#include "GUILayer.h"
#include "WindowEvents.h"
#include <cstdlib>

void gflw_error_callback(int code, const char* description)
{
    Larry::Ref<Larry::Events::GlfwErrorEvent> event = Larry::CreateRef<Larry::Events::GlfwErrorEvent>(description, code);
    Larry::EventSystem::HandleEvent(event);
}

namespace Larry {
     Application* Application::application = nullptr;

    Application* Application::GetApplication() {
        if (Application::application == nullptr) {
            LA_CORE_INFO("Creating application..."); 
            Application::application = new Application();
            application->OnCreate();
        }
        return application;
    }

    Application::Application()  {
        EventSystem::SetCallbackFunction(BIND_EVENT_FN(handleEvent));
    }

    Application::~Application() {

    }

    void Application::HandleEvent(const Ref<Event>& event) {
        Application::GetApplication()->handleEvent(event);
    }

    void Application::OnCreate() {
        glfwInit();
        // setting error callback
        glfwSetErrorCallback(gflw_error_callback);


        /* windowConfig.maximized = true; */
        window = CreateRef<LarryWindow>(windowConfig);
        renderer = Renderer::InitRenderer(rendererConfig, window);

        InitInput(window->GetWindow());

        layerStack.AttachLayer(CreateRef<BackgroundLayer>());
        layerStack.AttachLayer(CreateRef<GameLayer>());
        layerStack.AttachLayer(CreateRef<UILayer>());
        layerStack.AttachLayer(CreateRef<GUILayer>());
    }

    void Application::Run() {
        double lastFrameTime = glfwGetTime(); 
        while(running)
        {
            double time = glfwGetTime();
            double deltaTime = time - lastFrameTime;
            lastFrameTime = time;

            layerStack.UpdateLayers(deltaTime);

            renderer->UpdateFrame();

            lastFrameTime = glfwGetTime();
        }
    }

    void Application::handleEvent(const Ref<Event>& event) {
        switch (event->GetEventCategory()) {
            case EventCategory::Error:
                handleErrorEvent(event);
                break;
            case EventCategory::Input:
                HandleInputEvent(event);
                break;
            case EventCategory::Window:
                HandleWindowEvent(event);
                break;
            default:
                LA_CORE_INFO("Got an event of category {} and type {} and I dont know how t0 handle it.", event->GetEventCategory(), event->GetEventType());
                break;
        }

        if (!event->Handeled) {
            layerStack.PassEventToLayers(event);
        }
    }

    void Application::handleErrorEvent(const Ref<Event>& event) {
        bool dispatched = DispatchEvent<Events::ShaderCompilationFailedErrorEvent>(event, 
            [](const Ref<Event>& event){
                Events::ShaderCompilationFailedErrorEvent* e = (Events::ShaderCompilationFailedErrorEvent*)event.get();
                LA_CORE_ERROR("Shader compilation failed, error: {}", e->GetInfoLog());
                LA_CORE_INFO("Shader code:\n{}", e->GetShaderCode());
           });

        if (!dispatched) {
            Events::ErrorEvent* err = (Events::ErrorEvent*)event.get();
            LA_CORE_ERROR("Got an error: {}.", err->GetErrorMessage());
            if (err->IsFatal()) {
                event->Handeled = true;
                LA_CORE_INFO("Error is fatal terminating program.");
                running = false;
            }
        }
    }

    void Application::HandleInputEvent(const Ref<Event>& event) {
        LA_CORE_INFO("Got input event! type {}", event->GetEventType());
    }

    void Application::HandleWindowEvent(const Ref<Event>& event) {
        bool dispatched = DispatchEvent<Events::WindowResizedEvent>(event, [this](const Ref<Event>& event){
            Events::WindowResizedEvent* window_event = (Events::WindowResizedEvent*)event.get();
            LA_CORE_INFO("Windows resized to: ({}, {})", window_event->GetWidth(), window_event->GetHeight());
            renderer->SetViewPort(0, 0, window_event->GetWidth(), window_event->GetHeight());
        });

        dispatched = dispatched || DispatchEvent<Events::WindowCloseEvent>(event, [this](const Ref<Event>& event){
            Events::WindowCloseEvent* window_event = (Events::WindowCloseEvent*)event.get();
            LA_CORE_INFO("User pressed close button!");
            running = false;
        });

        if (!dispatched) {
            LA_CORE_INFO("Got a WindowEvent, event type: {}", event->GetEventType());
        }
    }
}
