#include "Application.h"
#include "Math.h"
#include "ErrorEvent.h"
#include "Event.h"
#include "EventSystem/EventSystem.h"
#include "GLFW/glfw3.h"
#include "LarryMemory.h"
#include "Log.h"
#include "ShaderCompilationFaildErrorEvent.h"
#include "TextureObject.h"
#include "WindowResizedEvent.h"
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
        EventSystem::SetCallbackFunction(Application::HandleEvent);
    }

    Application::~Application() {

    }

    void Application::HandleEvent(const Ref<Event>& event) {
        Application::GetApplication()->handleEvent(event);
    }

    void Application::OnCreate() {
        renderer = CreateScope<Renderer>(rendererConfig);
        TextureConfig config;
        config.CreateMipmap = false;
        face = CreateRef<TextureObject>("media/textures/awesomeface.png", config);
        test = CreateRef<TextureObject>("media/textures/test.jpg", TextureConfig{});
        wall = CreateRef<TextureObject>("media/textures/wall.jpg", TextureConfig{});
    }

    void Application::Run() {
        renderer->InitializeOrthographicProjection(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        renderer->View = Math::translate(renderer->View, Math::Vec3(0.0f, 0.0f, -3.0f));
        while(!renderer->ShouldClose())
        {
            double time = glfwGetTime() ;
            renderer->Background(Color{0.1f, 0.1f, 0.3f, 1});

            /* renderer->Texture(wall); */
            /* renderer->Scale(Math::vec3(sin(time)+2.0f * 3.0f, 1.01f, 1.0f)); */
            /* renderer->Rotate(cos(time) * 3.14); */
            /* renderer->Translate(Math::vec3(0.25f, -0.25f, 0.0f)); */
            /* renderer->Translate(Math::vec3(0.0, 0.0, -0.5f)); */
            /* renderer->Scale(Math::vec3(1.0f, 2.0f, 1.0f)); */
            /* renderer->Translate(Math::vec3(400.0f, 300.0f, 0.0f)); */
            renderer->Fill(1, 1, 1, 1);
            renderer->DrawQuad(200.0f, 300.0f, 200.0f, 200.0f);

            /* renderer->Texture(face); */
            /* renderer->Rotate((time)); */
            /* renderer->Translate(Math::vec3(-0.85f, -0.85f, 0.0f)); */
            /* renderer->DrawQuad(0.2f, 0.7f, 0.3f, 0.3f); */

            renderer->UpdateFrame();
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
    }

    void Application::handleErrorEvent(const Ref<Event>& event) {
        switch (event->GetEventType()) {
            case EventType::OpenGLShaderCompilationFailedError:
                {
                    Events::ShaderCompilationFailedErrorEvent* e = (Events::ShaderCompilationFailedErrorEvent*)event.get();
                    LA_CORE_ERROR("Shader compilation failed, error: {}", e->GetInfoLog());
                    LA_CORE_INFO("Shader code:\n{}", e->GetShaderCode());
                };
                break;
            default:
                {
                    Events::ErrorEvent* err = (Events::ErrorEvent*)event.get();
                    LA_CORE_ERROR("Got an error: {}.", err->GetErrorMessage());
                }
                break;
        }
        event->Handeled = true;
        LA_CORE_INFO("Terminating program.");
        exit(1);
    }

    void Application::HandleInputEvent(const Ref<Event>& event) {
        event->Handeled = true;
        LA_CORE_INFO("Got input event!");
    }

    void Application::HandleWindowEvent(const Ref<Event>& event) {
        switch (event->GetEventType()) {
            case EventType::WindowResized: 
                {
                    Events::WindowResizedEvent* window_event = (Events::WindowResizedEvent*)event.get();
                    LA_CORE_INFO("Windows resized to: ({}, {})", window_event->GetWidth(), window_event->GetHeight());
                    renderer->SetViewPort(0, 0, window_event->GetWidth(), window_event->GetHeight());
                    event->Handeled = true;
                }
                break;
            default:
                event->Handeled = true;
                LA_CORE_INFO("Got a WindowEvent, event type: {}", event->GetEventType());
                break;
        }
    }
}
