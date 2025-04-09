#include "UILayer.h"
#include "Layer.h"
#include "Renderer.h"

namespace Larry {

    UILayer::UILayer() : Layer("UILayer") {
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
