#include "gl.h"
#include "EventSystem/InputEvents.h"
#include "LarryApi.h"
#include "ECS_pch.h"
#include "common.h"
#include "ButtonSystem.h"
#include "Components/Button.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "Renderer.h"

namespace Larry {

    void ButtonSystem::OnCreate() {
        renderer = *world->GetSingelton<Renderer*>();
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

    void ButtonSystem::HandleMousePressedEvent(Events::MousePressedEvent* mouse_event) {
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

    void ButtonSystem::HandleEvent(Event* event) {
        DispatchEvent<Events::MousePressedEvent>(event, [this](Events::MousePressedEvent* e){ HandleMousePressedEvent(e); });
        DispatchEvent<Events::MouseMovedEvent>(event, [this](Events::MouseMovedEvent* mouse_event){
            mouseX = mouse_event->GetX();
            mouseY = mouse_event->GetY();
        });
    }
}
