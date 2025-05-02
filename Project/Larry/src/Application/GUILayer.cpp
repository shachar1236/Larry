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
        Layer::OnAttach();
    }

    void GUILayer::OnUpdate(const double& deltaTime) {
        Layer::OnUpdate(deltaTime);
    }

    void GUILayer::OnDetach() {
        Layer::OnDetach();
    }

    void GUILayer::HandleEvent(const Ref<Event>& event) {
        Layer::HandleEvent(event);
    }
}
