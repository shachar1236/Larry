#pragma once
#include "Scripts/lua/LuaScripts.h"
#include "Renderer/Renderer.h"
#include "EventSystem/Event.h"
#include "LarryWindow.h"
#include "LayerStack.h"
#include "Utils/LarryMemory.h"
#include "ECS/CPPApi/World.hpp"
#include <string>

namespace Larry {

    class Application {
        private:
            bool running = true;

            WindowConfig windowConfig;
            Ref<LarryWindow> window;
            RendererConfig rendererConfig;
            Renderer* renderer;
            LayerStack layerStack;
            Ref<ECS::World> ecs_world;
            Scripts::LuaScripts* lua_scripts;

            static Application* application;
        public:
            Application();
            ~Application();

            static Application* GetApplication();
            void OnCreate();
            void Run();

            void SaveScene();
            void GenerateScene(const std::string& scene_file_path);

            static void HandleEvent(const Ref<Event>&);
            void handleEvent(const Ref<Event>&);
            void handleErrorEvent(const Ref<Event>&);
            void HandleWindowEvent(const Ref<Event>&);
            void HandleInputEvent(const Ref<Event>&);
    };

}
