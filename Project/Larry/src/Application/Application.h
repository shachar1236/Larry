#pragma once
#include "EventSystem/Event.h"
#include "LayerStack.h"
#include "Log.h"
#include "LarryMemory.h"
#include "Renderer/Renderer.h"

namespace Larry {
    class Application {
        private:

            RendererConfig rendererConfig;
            Renderer* renderer;
            LayerStack layerStack;

            static Application* application;
        public:
            Application();
            ~Application();

            static Application* GetApplication();
            void OnCreate();
            void Run();
            
            static void HandleEvent(const Ref<Event>&);
            void handleEvent(const Ref<Event>&);
            void handleErrorEvent(const Ref<Event>&);
            void HandleWindowEvent(const Ref<Event>&);
            void HandleInputEvent(const Ref<Event>&);
    };

}
