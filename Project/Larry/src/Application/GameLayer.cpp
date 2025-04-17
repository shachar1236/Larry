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

        TextureConfig config;
        config.CreateMipmap = false;
        face = CreateRef<TextureObject>("media/textures/awesomeface.png", config);
        test = CreateRef<TextureObject>("media/textures/test.jpg", TextureConfig{});
        wall = CreateRef<TextureObject>("media/textures/wall.jpg", TextureConfig{});

        entity1 = world->CreateEntity();
        entity2 = world->CreateEntity();

        world->InsertComponent<Transform, Quad>(entity1, [=, this](Transform& transform, Quad& quad){
            transform = Transform();
            transform.translation.x = 0.7;
            transform.translation.y = 0.3;

            quad = Quad();
            quad.dimentions.x = 0.3;
            quad.dimentions.y = 0.3;
            /* quad.color = Math::Vec4(0.4, 0.2, 0.7, 1.0f); */
            quad.texture = face;
        });

        world->InsertComponent<Transform, Quad>(entity2, [=](Transform& transform, Quad& quad){
            transform = Transform();
            transform.translation.x = 0.4;
            transform.translation.y = 0.7;

            quad = Quad();
            quad.dimentions.x = 0.5;
            quad.dimentions.y = 0.5;
            quad.color = Math::Vec4(0.2, 0.4, 0.3, 1.0f);
        });
    }

    GameLayer::~GameLayer() {

    }

    void GameLayer::OnAttach() {
        renderer->InitializeOrthographicProjection(0.0f, 1.0f, 0.0f, 1.0f, 0.1f, 100.0f);
        renderer->View = Math::translate(renderer->View, Math::Vec3(0.0f, 0.0f, -3.0f));
    }

    void GameLayer::OnUpdate(const double& deltaTime) {
        /* position += moveDirection; */
        /* renderer->Fill(1, 1, 1, 1); */
        /* renderer->DrawQuad(position.x, position.y, 200.0f, 200.0f); */
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

        switch (keyEvent->GetKey()) {
            case KEY_W:
                moveDirection.y += speed;
                break;
            case KEY_S:
                moveDirection.y -= speed;
                break;
            case KEY_D:
                moveDirection.x += speed;
                break;
            case KEY_A:
                moveDirection.x -= speed;
                break;
        }
    }

    void GameLayer::HandleKeyReleased(const Ref<Event>& event) {
        Events::KeyReleasedEvent* keyEvent = (Events::KeyReleasedEvent*)event.get();

        switch (keyEvent->GetKey()) {
            case KEY_W:
                moveDirection.y = 0;
                break;
            case KEY_S:
                moveDirection.y = 0;
                break;
            case KEY_D:
                moveDirection.x = 0;
                break;
            case KEY_A:
                moveDirection.x = 0;
                break;
        }
    }

    void GameLayer::HandleMouseMoved(const Ref<Event>& event) {
        if (mouse_pressed) {
            Events::MouseMovedEvent* mouse_event = (Events::MouseMovedEvent*)event.get();
            this->position.x = mouse_event->GetX();
            this->position.y = mouse_event->GetY();
        }
    }

    void GameLayer::HandleMousePressed(const Ref<Event>& event) {
        mouse_pressed = true;
    }

    void GameLayer::HandleMouseReleased(const Ref<Event>& event) {
        mouse_pressed = false;
    }
}
