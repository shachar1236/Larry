#pragma once
#include "EventSystem/Event.h"
#include "Log.h"
#include "LarryMemory.h"
#include "Renderer/Renderer.h"

namespace Larry {
    class Application {
        private:
            Ref<TextureObject> face; //("media/textures/wal.jpg", TextureConfig{})
            Ref<TextureObject> test; //("media/textures/wal.jpg", TextureConfig{})
            Ref<TextureObject> wall; //("media/textures/wal.jpg", TextureConfig{})

            RendererConfig rendererConfig;
            Scope<Renderer> renderer;

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
