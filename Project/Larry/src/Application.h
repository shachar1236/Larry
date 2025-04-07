#pragma once
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
            Renderer renderer;
        public:
            Application();
            ~Application();

            void Run();
    };
}
