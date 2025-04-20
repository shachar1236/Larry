#include "GameLayer.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "EventSystem.h"
#include "InputEvents.h"
#include "KeyCodes.h"
#include "Layer.h"
#include "Log.h"
#include "Math.h"
#include "Renderer.h"

namespace Larry {

    GameLayer::GameLayer(const Ref<ECS::World>& world_) : Layer("GameLayer", world_) {
        renderer = Renderer::GetRenderer();
        viewport_size = renderer->GetViewportSize();
    }

    GameLayer::~GameLayer() {

    }

    void GameLayer::OnAttach() {
        renderer->InitializeOrthographicProjection(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        renderer->View = Math::translate(renderer->View, Math::Vec3(0.0f, 0.0f, -3.0f));
    }

    void GameLayer::OnUpdate(const double& deltaTime) {
        Layer::OnUpdate(deltaTime);
    }

    void GameLayer::OnDetach() {
        Layer::OnDetach();
    }

    void GameLayer::HandleEvent(const Ref<Event>& event) {
        Layer::HandleEvent(event);

        DispatchEvent<Events::KeyPressedEvent>(event, BIND_EVENT_FN(HandleKeyPressed));
        DispatchEvent<Events::KeyReleasedEvent>(event, BIND_EVENT_FN(HandleKeyReleased));

        DispatchEvent<Events::MousePressedEvent>(event, BIND_EVENT_FN(HandleMousePressed));
        DispatchEvent<Events::MouseReleasedEvent>(event, BIND_EVENT_FN(HandleMouseReleased));
        DispatchEvent<Events::MouseMovedEvent>(event, BIND_EVENT_FN(HandleMouseMoved));
    }

    void GameLayer::HandleKeyPressed(const Ref<Event>& event) {
        Events::KeyPressedEvent* keyEvent = (Events::KeyPressedEvent*)event.get();

    }

    void GameLayer::HandleKeyReleased(const Ref<Event>& event) {
        Events::KeyReleasedEvent* keyEvent = (Events::KeyReleasedEvent*)event.get();
    }

    void GameLayer::HandleMouseMoved(const Ref<Event>& event) {
    }

    void GameLayer::HandleMousePressed(const Ref<Event>& event) {
    }

    void GameLayer::HandleMouseReleased(const Ref<Event>& event) {
    }
}
