#include "ButtonSystem.h"
#include "Components/Button.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "EventSystem.h"
#include "InputEvents.h"
#include "Log.h"
#include "Renderer.h"
#include "World.hpp"

namespace Larry {

    void ButtonSystem::OnCreate() {
        renderer = *world->GetSingelton<Renderer*>().value();
    }

    void ButtonSystem::OnUpdate(double deltaTime) {
        world->System<Transform, Button>([this](Transform& transform, Button& button){
            renderer->Translate(transform.translation);
            renderer->Rotate(transform.rotation_size, transform.rotation_axis);
            renderer->Scale(transform.scale);
            renderer->Fill(button.color);
            renderer->DrawQuad(button.dimentions);
        });
    }

    void ButtonSystem::OnDelete() {

    }

    void ButtonSystem::HandleMousePressedEvent(const Ref<Event>& event) {
        Events::MousePressedEvent* mouse_event = (Events::MousePressedEvent*)(event.get());
        world->System<Transform, Button>([this, mouse_event](Transform& transform, Button& button){
            if (!mouse_event->Handeled) {
                int realX = transform.translation.x - button.dimentions.x / 2;
                int realY = transform.translation.y - button.dimentions.y / 2;
                int realXEnd = transform.translation.x + button.dimentions.x / 2;
                int realYEnd = transform.translation.y + button.dimentions.y / 2;
                bool colides = mouseX < realXEnd && mouseX > realX && mouseY < realYEnd && mouseY > realY;
                if (colides) {
                    LA_CORE_DEBUG("Button has been pressed!");
                    mouse_event->Handeled = true;
                }
            }
        });
    }

    void ButtonSystem::HandleEvent(const Ref<Event>& event) {
        DispatchEvent<Events::MousePressedEvent>(event, [this](const Ref<Event>& e){ HandleMousePressedEvent(e); });
        DispatchEvent<Events::MouseMovedEvent>(event, [this](const Ref<Event>& e){
            Events::MouseMovedEvent* mouse_event = (Events::MouseMovedEvent*)(e.get());
            mouseX = mouse_event->GetX();
            mouseY = mouse_event->GetY();
        });
    }
}
