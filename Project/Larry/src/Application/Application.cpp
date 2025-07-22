#include "Components/Background.h"
#include "Components/Button.h"
#include "ECS/CPPApi/ECS.h"
#include "ECS_pch.h"
#include "Application/Application.h"
#include "Components/Camera.h"
#include "Components/Projection.h"
#include "LuaScripts.h"
#include "Math.h"
#include "Renderer.h"
#include "ScriptsManager.h"
#include "SystemEvents.h"
#include "Systems/RelationsSystem.h"
#include "World.hpp"
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
#include "yaml-cpp/node/node.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <unordered_map>

#define SetComponentIfExists(component, ...) \
    if (entity_componenets[#component]) { \
        ecs_world->InsertComponent<component>(ecs_entity, [&](component& comp){ \
            comp.DecodeYAML(entity_componenets[#component] __VA_OPT__(,) __VA_ARGS__); \
        }); \
    } \

#define InsertComponentToEntites(component, ...) \
    ecs_world->AdvancedSystem<component>([this, &world](ECS::Entity entity, bool* stop, component& comp){ \
        std::string name = ecs_world->GetEntityName(entity).value(); \
        world[name][#component] = comp.EncodeYAML(__VA_ARGS__); \
    }); \

#define RegisterComponentOnLuaScripts(type) lua_scripts->RegisterComponent(#type, TypeHash(type))

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

        ecs_world = CreateRef<ECS::World>();

        window = CreateRef<LarryWindow>(windowConfig);
        renderer = Renderer::InitRenderer(rendererConfig, window);

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

        static_cast<Layer*>(layerStack.GetLayer("GameLayer").get())->AddSystem(CreateRef<RelationSystem>(ecs_world));
        static_cast<Layer*>(layerStack.GetLayer("GameLayer").get())->AddSystem(CreateRef<RenderQuad>(ecs_world));
        static_cast<Layer*>(layerStack.GetLayer("UILayer").get())->AddSystem(CreateRef<ButtonSystem>(ecs_world));

        Scripts::RegisterScripts(ecs_world, layerStack);

        Scripts::LuaScripts::InitLuaScripts(ecs_world);
        lua_scripts = Scripts::LuaScripts::GetInstance();

        RegisterComponentOnLuaScripts(Background);
        RegisterComponentOnLuaScripts(Button);
        RegisterComponentOnLuaScripts(Camera);
        RegisterComponentOnLuaScripts(Projection);
        RegisterComponentOnLuaScripts(Quad);
        RegisterComponentOnLuaScripts(Parent);
        RegisterComponentOnLuaScripts(Child);
        RegisterComponentOnLuaScripts(Transform);

        Input::Init(window->GetWindow());

        GenerateScene("config.yaml");

        Scripts::Script::Init(ecs_world);

        lua_scripts->test(ecs_world->GetInternalWorld());

        ecs_world->SetComponents<Transform, Quad>(3, [](Transform& transform, Quad& quad){
            LA_CORE_INFO("Entity transform translation ({}, {}, {})", transform.translation.x, transform.translation.y, transform.translation.z);
            LA_CORE_INFO("Entity transform scale ({}, {}, {})", transform.scale.x, transform.scale.y, transform.scale.z);
            LA_CORE_INFO("Entity rotation axis scale ({}, {}, {})", transform.rotation_axis.x, transform.rotation_axis.y, transform.rotation_axis.z);
            LA_CORE_INFO("Entity rotation size {}", transform.rotation_size);
            LA_CORE_INFO("Entity texture {}", (void*)quad.texture);
        });

        EventSystem::HandleEvent(CreateRef<Events::SystemInitEvent>());
        // SaveScene();
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

            lua_scripts->UpdateScripts(ecs_world->GetInternalWorld(), deltaTime);

            renderer->UpdateFrame();
        }
    }

    void Application::SaveScene() {
        YAML::Emitter out;

        YAML::Node config;
        config["options"]["window"] = windowConfig;

        config["textures"] = *ecs_world->GetSingelton<TextureLoader>();
        
        YAML::Node world;

        InsertComponentToEntites(Transform);
        InsertComponentToEntites(Quad);
        InsertComponentToEntites(Background);
        InsertComponentToEntites(Button);
        InsertComponentToEntites(Camera);
        InsertComponentToEntites(Projection);

        ecs_world->AdvancedSystem<Parent>([this, &world](ECS::Entity entity, bool* stop, Parent& p){ 
            std::string name = ecs_world->GetEntityName(entity).value();
            std::optional<ECS::Entity> child = p.firstChild;
            while (child.has_value()) {
                std::string child_name = ecs_world->GetEntityName(child.value()).value();

                world[name]["Childrens"].push_back(child_name);

                ecs_world->SetComponents<Child>(child.value(), [&child](Child& c){
                    child = c.nextChild;
                });
            }
        });

        ECS::Internal::World* iworld = ecs_world->GetInternalWorld();

        ECS::Internal::AnyQueue* any_queue = iworld->InitAnyQueue();
        ECS::Internal::TypeQueue* type_queue = iworld->InitTypeQueue();

        for (auto&& s : Scripts::Script::scriptName_to_detailes) {
            ECS_TypeHashCode hash_code = s.second.hash_code;

            any_queue->Clear();
            type_queue->Clear();

            type_queue->push_back(hash_code);

            iworld->System(*type_queue, *any_queue, [this, &world, &s](ECS_Entity entity, ECS::Internal::AnyQueue& components, bool* stop){
                std::string name = ecs_world->GetEntityName(entity).value();
                world[name]["Scripts"].push_back(s.first);
            });
        }

        iworld->DoneWithAnyQueue(any_queue);
        iworld->DoneWithTypeQueue(type_queue);
        
        // TODO: save lua scripts

        config["world"] = world;

        out << config;

        std::ofstream save_file("config.yaml");
        save_file << out.c_str();
    }

    void Application::GenerateScene(const std::string& scene_file_path) {
        YAML::Node config = YAML::LoadFile(scene_file_path);
        TextureLoader* texture_loader = ecs_world->GetSingelton<TextureLoader>();
        if (config["textures"] && config["textures"].IsMap()) {
            *texture_loader = config["textures"].as<TextureLoader>();
        }

        windowConfig = config["options"]["window"].as<WindowConfig>();

        std::unordered_map<std::string, ECS::Entity> entites;

        YAML::Node world = config["world"];
        if (world.IsMap()) {
            for (auto&& entity : world) {
                std::string entity_name = entity.first.as<std::string>();
                ECS::Entity ecs_entity = ecs_world->CreateEntity(entity_name);

                entites[entity_name] = ecs_entity;

                YAML::Node entity_componenets = entity.second;
                if (entity_componenets.IsMap()) {
                    SetComponentIfExists(Transform);
                    SetComponentIfExists(Quad, *texture_loader);
                    SetComponentIfExists(Background, *texture_loader);
                    SetComponentIfExists(Button);
                    SetComponentIfExists(Camera);
                    SetComponentIfExists(Projection);

                    if (entity_componenets["Scripts"]) {
                        YAML::Node scripts = entity_componenets["Scripts"];
                        for (int i = 0; i < scripts.size(); i++) {
                            Scripts::AddScriptToEntity(ecs_entity, scripts[i].as<std::string>(), ecs_world);
                        }
                    }
                    if (entity_componenets["LuaScripts"]) {
                        YAML::Node scripts = entity_componenets["LuaScripts"];
                        for (int i = 0; i < scripts.size(); i++) {
                            lua_scripts->AddScriptToEntity(scripts[i].as<std::string>(), ecs_world, ecs_entity);
                        }
                    }
                }
            }

            for (auto&& entity : world) {
                std::string entity_name = entity.first.as<std::string>();
                ECS::Entity ecs_entity = entites[entity_name];

                YAML::Node entity_componenets = entity.second;
                if (entity_componenets.IsMap() && entity_componenets["Childrens"]) {
                    YAML::Node childrens = entity_componenets["Childrens"];
                    if (childrens.IsSequence()) {
                        for (int i = 0; i < childrens.size(); i++) {
                            auto res = entites.find(childrens[i].as<std::string>());
                            if (res != entites.end()) {
                                AddChild(ecs_world, ecs_entity, res->second);
                            }
                        }
                    }
                }
            }
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
                LA_CORE_WARN("Got an event of category {} and type {} and I dont know how to handle it.", event->GetEventCategory(), event->GetEventType());
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
