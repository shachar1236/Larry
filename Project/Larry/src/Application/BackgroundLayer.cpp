#include "BackgroundLayer.h"
#include "Layer.h"
#include "Renderer.h"

namespace Larry {

    BackgroundLayer::BackgroundLayer() : Layer("BackgroundLayer") {
    }

    BackgroundLayer::~BackgroundLayer() {

    }

    void BackgroundLayer::OnAttach() {
        renderer = Renderer::GetRenderer();
    }

    void BackgroundLayer::OnUpdate(const double& deltaTime) {
        renderer->Background(Color{0.1f, 0.1f, 0.3f, 1});
    }

    void BackgroundLayer::OnDetach() {

    }

    void BackgroundLayer::HandleEvent(const Ref<Event>& event) {

    }
}
