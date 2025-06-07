#include "ECS.h"
#include "ECS_pch.h"
#include "Application/Application.h"
#include "Components/Camera.h"
#include "Components/Projection.h"
#include "Math.h"
#include "Renderer.h"
#include "ScriptsManager.h"
#include "Systems/RelationsSystem.h"
#include "common.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "Entity.hpp"
#include "ErrorEvents.h"
#include "Event.h"
#include "EventSystem/EventSystem.h"
#include "LarryMemory.h"
#include "LarryWindow.h"
#include "Log.h"
#include "LayerStack.h"
#include "BackgroundLayer.h"
#include "GameLayer.h"
#include "Scripts/Scripts.h"
#include "Systems/ButtonSystem.h"
#include "Systems/RenderQuad.h"
#include "Systems/ScriptsSystem.h"
#include "Components/Relationship.h"
#include "TextureLoader/TextureLoader.h"
#include "UILayer.h"
#include "GUILayer.h"
#include "WindowEvents.h"
#include "gl.h"
#include "Input.h"
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
        EventSystem::AddCallbackFunction(BIND_EVENT_FN(handleEvent));
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

        Input::Init(window->GetWindow());

        ecs_world = CreateRef<ECS::World>();
        ecs_world->SetSingelton<Renderer*>([this](Renderer*& rend){
            rend = renderer;
        });
        ecs_world->SetSingelton<Ref<LarryWindow>>([this](Ref<LarryWindow>& win){
            win = window;
        });

        ecs_world->SetSingelton<LayerStack*>([this](LayerStack*& layer_stack){
            layer_stack = &this->layerStack;
        });

        layerStack.AttachLayer(CreateRef<BackgroundLayer>(ecs_world));
        layerStack.AttachLayer(CreateRef<GameLayer>(ecs_world));
        layerStack.AttachLayer(CreateRef<UILayer>(ecs_world));
        layerStack.AttachLayer(CreateRef<GUILayer>(ecs_world));

        Scripts::RegisterScripts(ecs_world, layerStack);

        static_cast<Layer*>(layerStack.GetLayer("GameLayer").get())->AddSystem(CreateRef<RelationSystem>(ecs_world));
        static_cast<Layer*>(layerStack.GetLayer("GameLayer").get())->AddSystem(CreateRef<RenderQuad>(ecs_world));
        static_cast<Layer*>(layerStack.GetLayer("UILayer").get())->AddSystem(CreateRef<ButtonSystem>(ecs_world));

        GenerateScene("test");
        Scripts::Script::Init(ecs_world);
    }

    void Application::Run() {
        double lastFrameTime = glfwGetTime(); 
        while(running)
        {
            double time = glfwGetTime();
            double deltaTime = time - lastFrameTime;
            lastFrameTime = time;

            glfwPollEvents();
            EventSystem::HandleQueuedEvents();

            layerStack.UpdateLayers(deltaTime);

            renderer->UpdateFrame();
        }
    }


    void Application::GenerateScene(const std::string& scene_file_path) {
        // TODO: load file and generate scene
        ECS::Entity entity1 = ecs_world->CreateEntity();
        ecs_world->InsertComponent<Transform, Quad>(entity1, [](Transform& transform, Quad& quad){
            transform.translation.x = 400;
            transform.translation.y = 300;

            quad.dimentions.x = 200;
            quad.dimentions.y = 200;
        });

        /* ecs_world->InsertComponent<Scripts::ScriptsComponent>(entity1, [this, entity1](Scripts::ScriptsComponent& scripts){
            Ref<Scripts::Script> testScript = Scripts::Script::GetNewInstanceOfScript("TestScript", ecs_world);
            scripts.push_back(testScript);
            testScript->OnCreate(entity1);
        }); */
        Scripts::AddScriptToEntity(entity1, "TestScript", ecs_world);

        ECS::Entity entity2 = ecs_world->CreateEntity();
        ecs_world->InsertComponent<Transform, Quad>(entity2, [](Transform& transform, Quad& quad){
            transform.translation.x = 100;
            transform.translation.y = 100;

            quad.dimentions.x = 100;
            quad.dimentions.y = 100;

            quad.color = Math::Vec4(0.3f, 0.8f, 0.4f, 1.0f);
        });
        AddChild(ecs_world, entity1, entity2);

        ECS::Entity entity3 = ecs_world->CreateEntity();
        ecs_world->InsertComponent<Transform, Quad>(entity3, [](Transform& transform, Quad& quad){
            transform.translation.x = 0;
            transform.translation.y = 0;

            quad.dimentions.x = 100;
            quad.dimentions.y = 100;

            quad.color = Math::Vec4(0.3f, 0.8f, 0.4f, 1.0f);
        });
        AddChild(ecs_world, entity1, entity3);

        ECS::Entity entity4 = ecs_world->CreateEntity();
        ecs_world->InsertComponent<Transform, Quad>(entity4, [](Transform& transform, Quad& quad){
            transform.translation.x = 100;
            transform.translation.y = 100;

            quad.dimentions.x = 50;
            quad.dimentions.y = 50;

            quad.color = Math::Vec4(0.3f, 0.8f, 0.4f, 1.0f);
        });

        ECS::Entity proj_entity = ecs_world->CreateEntity();
        ecs_world->InsertComponent<Projection>(proj_entity, [this](Projection& proj){
            proj = Projection();
            proj.projection = Math::ortho(0.0f, windowConfig.window_width, 0.0f, windowConfig.window_height, 0.1f, 100.0f);
            for (auto& layer : layerStack.layers) {
                proj.projection_layers.insert(layer->GetId());
            }
        });

        ECS::Entity camera_entity = ecs_world->CreateEntity();
        ecs_world->InsertComponent<Camera>(camera_entity, [this](Camera& camera){
            camera = Camera();
            for (auto& layer : layerStack.layers) {
                if (layer->GetName() != "GameLayer") {
                    camera.view_layers.insert(layer->GetId());
                }
            }
        });
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
                LA_CORE_INFO("Got an event of category {} and type {} and I dont know how to handle it.", event->GetEventCategory(), event->GetEventType());
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
