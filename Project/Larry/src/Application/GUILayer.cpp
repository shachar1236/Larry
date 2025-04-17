#include "GUILayer.h"
#include "Layer.h"
#include "Renderer.h"

namespace Larry {

    GUILayer::GUILayer(const Ref<ECS::World>& world_) : Layer("GUILayer", world_) {
    }

    GUILayer::~GUILayer() {

    }

    void GUILayer::OnAttach() {
        renderer = Renderer::GetRenderer();
    }

    void GUILayer::OnUpdate(const double& deltaTime) {
    }

    void GUILayer::OnDetach() {

    }

    void GUILayer::HandleEvent(const Ref<Event>& event) {

    }
}
