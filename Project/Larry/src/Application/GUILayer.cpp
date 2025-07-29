#include "gl.h"
#include "LarryApi.h"
#include "ECS_pch.h"
#include "common.h"
#include "GUILayer.h"
#include "Renderer.h"

namespace Larry {

    GUILayer::GUILayer(ECS::World* world_) : Layer("GUILayer", world_) {
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

    void GUILayer::HandleEvent(Event* event) {
        Layer::HandleEvent(event);
    }
}
