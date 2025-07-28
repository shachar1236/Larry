#include "LarryApi.h"
#include "ECS_pch.h"
#include "common.h"
#include "UILayer.h"
#include "Components/Button.h"
#include "Components/Transform.h"
#include "Renderer.h"

namespace Larry {

    UILayer::UILayer(const Ref<ECS::World>& world_) : Layer("UILayer", world_) {
    }

    UILayer::~UILayer() {

    }

    void UILayer::OnAttach() {
        button = world->CreateEntity("button1");

        Layer::OnAttach();
    }

    void UILayer::OnUpdate(const double& deltaTime) {
        Layer::OnUpdate(deltaTime);
    }

    void UILayer::OnDetach() {
        Layer::OnDetach();
    }

    void UILayer::HandleEvent(const Ref<Event>& event) {
        Layer::HandleEvent(event);
    }
}
