#include "BackgroundLayer.h"
#include "Event.h"
#include "EventSystem.h"
#include "Layer.h"
#include "Renderer.h"
#include "WindowEvents.h"

namespace Larry {

    BackgroundLayer::BackgroundLayer(const Ref<ECS::World>& world_) : Layer("BackgroundLayer", world_) {
    }

    BackgroundLayer::~BackgroundLayer() {

    }

    void BackgroundLayer::OnAttach() {
        color = {0.1f, 0.1f, 0.3f, 1};
        Layer::OnAttach();
    }

    void BackgroundLayer::OnUpdate(const double& deltaTime) {
        Layer::OnUpdate(deltaTime);
        renderer->Background(color);
    }

    void BackgroundLayer::OnDetach() {
        Layer::OnDetach();
    }

    void BackgroundLayer::HandleEvent(const Ref<Event>& event) {
        bool dispatched = DispatchEvent<Events::WindowResizedEvent>(event, EVENT_LAMBDA(this, {
            color.r += 0.05;
        }));
        Layer::HandleEvent(event);
    }
}
