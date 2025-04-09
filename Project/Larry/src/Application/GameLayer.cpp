#include "GameLayer.h"
#include "EventSystem.h"
#include "InputEvents.h"
#include "KeyCodes.h"
#include "Layer.h"
#include "Log.h"
#include "Renderer.h"

namespace Larry {

    GameLayer::GameLayer() : Layer("GameLayer") {
        renderer = Renderer::GetRenderer();
        TextureConfig config;
        config.CreateMipmap = false;
        face = CreateRef<TextureObject>("media/textures/awesomeface.png", config);
        test = CreateRef<TextureObject>("media/textures/test.jpg", TextureConfig{});
        wall = CreateRef<TextureObject>("media/textures/wall.jpg", TextureConfig{});
    }

    GameLayer::~GameLayer() {

    }

    void GameLayer::OnAttach() {
        renderer->InitializeOrthographicProjection(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        renderer->View = Math::translate(renderer->View, Math::Vec3(0.0f, 0.0f, -3.0f));
    }

    void GameLayer::OnUpdate(const double& deltaTime) {
        position += moveDirection;
        renderer->Fill(1, 1, 1, 1);
        renderer->DrawQuad(position.x, position.y, 200.0f, 200.0f);
    }

    void GameLayer::OnDetach() {

    }

    void GameLayer::HandleEvent(const Ref<Event>& event) {
        DispatchEvent<Events::KeyPressedEvent>(event, BIND_EVENT_FN(HandleKeyPressed));
        DispatchEvent<Events::KeyReleasedEvent>(event, BIND_EVENT_FN(HandleKeyReleased));
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
}
