#include "Log.h"
#include "Renderer/Renderer.h"

namespace Larry {
    class Application {
        private:
            RendererConfig rendererConfig;
            Renderer renderer;
        public:
            Application();
            ~Application();

            void Run();
    };
}
