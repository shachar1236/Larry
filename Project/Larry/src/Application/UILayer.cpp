#include "UILayer.h"
#include "Components/Button.h"
#include "Components/Transform.h"
#include "Layer.h"
#include "Math.h"
#include "Renderer.h"

namespace Larry {

    UILayer::UILayer(const Ref<ECS::World>& world_) : Layer("UILayer", world_) {
    }

    UILayer::~UILayer() {

    }

    void UILayer::OnAttach() {
        button = world->CreateEntity();

        world->InsertComponent<Transform, Button>(button, [](Transform& transform, Button& b){
            transform = Transform();
            transform.translation.x = 600;
            transform.translation.y = 300;

            b = Button();
            b.dimentions = Math::Vec2(50.0f);
            b.color = Math::Vec4(0.4, 0.8, 0.8, 1.0);
        });
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
