#include "LarryApi.h"
#include "common.h"
#include "GameLayer.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "Renderer.h"

namespace Larry {

    GameLayer::GameLayer(ECS::World* world_) : Layer("GameLayer", world_) {
    }

    GameLayer::~GameLayer() {

    }

    void GameLayer::OnAttach() {
        /* renderer->InitializeOrthographicProjection(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f); */
        renderer->View = Math::translate(renderer->View, Math::Vec3(0.0f, 0.0f, -3.0f));
        Layer::OnAttach();
    }

    void GameLayer::OnUpdate(const double& deltaTime) {
        Layer::OnUpdate(deltaTime);
    }

    void GameLayer::OnDetach() {
        Layer::OnDetach();
    }

    void GameLayer::HandleEvent(Event* event) {
        Layer::HandleEvent(event);

        DispatchEvent<Events::KeyPressedEvent>(event, [&](Events::KeyPressedEvent *e) { this->HandleKeyPressed(e); });
        DispatchEvent<Events::KeyReleasedEvent>(event, [&](Events::KeyReleasedEvent *e) { this->HandleKeyReleased(e); });

        DispatchEvent<Events::MousePressedEvent>(event, [&](Events::MousePressedEvent *e) { this->HandleMousePressed(e); });
        DispatchEvent<Events::MouseReleasedEvent>(event, [&](Events::MouseReleasedEvent *e) { this->HandleMouseReleased(e); });
        DispatchEvent<Events::MouseMovedEvent>(event, [&](Events::MouseMovedEvent *e) { this->HandleMouseMoved(e); });
    }

    void GameLayer::HandleKeyPressed(Events::KeyPressedEvent* keyEvent) {

    }

    void GameLayer::HandleKeyReleased(Events::KeyReleasedEvent* keyEvent) {
    }

    void GameLayer::HandleMouseMoved(Events::MouseMovedEvent* event) {
    }

    void GameLayer::HandleMousePressed(Events::MousePressedEvent* event) {
    }

    void GameLayer::HandleMouseReleased(Events::MouseReleasedEvent* event) {
    }
}
