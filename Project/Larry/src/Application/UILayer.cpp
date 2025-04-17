#include "UILayer.h"
#include "Layer.h"
#include "Renderer.h"

namespace Larry {

    UILayer::UILayer(const Ref<ECS::World>& world_) : Layer("UILayer", world_) {
    }

    UILayer::~UILayer() {

    }

    void UILayer::OnAttach() {
        renderer = Renderer::GetRenderer();
    }

    void UILayer::OnUpdate(const double& deltaTime) {
    }

    void UILayer::OnDetach() {

    }

    void UILayer::HandleEvent(const Ref<Event>& event) {

    }
}
