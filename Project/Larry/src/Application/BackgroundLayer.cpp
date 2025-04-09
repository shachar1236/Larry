#include "BackgroundLayer.h"
#include "Event.h"
#include "EventSystem.h"
#include "Layer.h"
#include "Renderer.h"
#include "WindowResizedEvent.h"

namespace Larry {

    BackgroundLayer::BackgroundLayer() : Layer("BackgroundLayer") {
    }

    BackgroundLayer::~BackgroundLayer() {

    }

    void BackgroundLayer::OnAttach() {
        renderer = Renderer::GetRenderer();
        color = {0.1f, 0.1f, 0.3f, 1};
    }

    void BackgroundLayer::OnUpdate(const double& deltaTime) {
        renderer->Background(color);
    }

    void BackgroundLayer::OnDetach() {

    }

    void BackgroundLayer::HandleEvent(const Ref<Event>& event) {
        bool dispatched = DispatchEvent<Events::WindowResizedEvent>(event, EVENT_LAMBDA(this, {
            color.r += 0.05;
        }));
    }
}
