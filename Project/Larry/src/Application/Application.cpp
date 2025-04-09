#include "Application.h"
#include "ErrorEvent.h"
#include "Event.h"
#include "EventSystem/EventSystem.h"
#include "GLFW/glfw3.h"
#include "LarryMemory.h"
#include "Log.h"
#include "ShaderCompilationFaildErrorEvent.h"
#include "WindowResizedEvent.h"
#include "LayerStack.h"
#include "BackgroundLayer.h"
#include "GameLayer.h"
#include "UILayer.h"
#include "GUILayer.h"
#include <cstdlib>

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
        renderer = Renderer::InitRenderer(rendererConfig);

        layerStack.AttachLayer(CreateRef<BackgroundLayer>());
        layerStack.AttachLayer(CreateRef<GameLayer>());
        layerStack.AttachLayer(CreateRef<UILayer>());
        layerStack.AttachLayer(CreateRef<GUILayer>());
    }

    void Application::Run() {
        double lastFrameTime = glfwGetTime(); 
        while(!renderer->ShouldClose())
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
        }
        event->Handeled = true;
        LA_CORE_INFO("Terminating program.");
        exit(1);
    }

    void Application::HandleInputEvent(const Ref<Event>& event) {
        LA_CORE_INFO("Got input event!");
    }

    void Application::HandleWindowEvent(const Ref<Event>& event) {
        bool dispatched = DispatchEvent<Events::WindowResizedEvent>(event, [this](const Ref<Event>& event){
            Events::WindowResizedEvent* window_event = (Events::WindowResizedEvent*)event.get();
            LA_CORE_INFO("Windows resized to: ({}, {})", window_event->GetWidth(), window_event->GetHeight());
            renderer->SetViewPort(0, 0, window_event->GetWidth(), window_event->GetHeight());
        });

        if (!dispatched) {
            LA_CORE_INFO("Got a WindowEvent, event type: {}", event->GetEventType());
        }
    }
}
