#include "GameLayer.h"
#include "Layer.h"
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
        /* renderer->Texture(wall); */
        /* renderer->Scale(Math::vec3(sin(time)+2.0f * 3.0f, 1.01f, 1.0f)); */
        /* renderer->Rotate(cos(time) * 3.14); */
        /* renderer->Translate(Math::vec3(0.25f, -0.25f, 0.0f)); */
        /* renderer->Translate(Math::vec3(0.0, 0.0, -0.5f)); */
        /* renderer->Scale(Math::vec3(1.0f, 2.0f, 1.0f)); */
        /* renderer->Translate(Math::vec3(400.0f, 300.0f, 0.0f)); */
        renderer->Fill(1, 1, 1, 1);
        renderer->DrawQuad(200.0f, 300.0f, 200.0f, 200.0f);

        /* renderer->Texture(face); */
        /* renderer->Rotate((time)); */
        /* renderer->Translate(Math::vec3(-0.85f, -0.85f, 0.0f)); */
        /* renderer->DrawQuad(0.2f, 0.7f, 0.3f, 0.3f); */
    }

    void GameLayer::OnDetach() {

    }

    void GameLayer::HandleEvent(const Ref<Event>& event) {

    }
}
