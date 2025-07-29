#include "gl.h"
#include "ECS_pch.h"
#include "common.h"
#include "BackgroundLayer.h"
#include "Components/Background.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventSystem.h"
#include "Layer/Layer.h"
#include "Utils/Log.h"
#include "Math/Math.h"
#include "Renderer.h"
#include "EventSystem/WindowEvents.h"

namespace Larry {

    BackgroundLayer::BackgroundLayer(ECS::World* world_) : Layer("BackgroundLayer", world_) {
    }

    BackgroundLayer::~BackgroundLayer() {
    }

    void BackgroundLayer::OnAttach() {
        Layer::OnAttach();
    }

    void BackgroundLayer::OnUpdate(const double& deltaTime) {
        Layer::OnUpdate(deltaTime);
        Background* bg = world->GetSingelton<Background>();
        renderer->Background(bg->color);

        if (bg->texture != nullptr) {
            LA_CORE_INFO("Background layer setting textured background");
            renderer->Translate(Math::Vec3(width/2, height/2, 0.0f));
            renderer->Scale(Math::Vec3(1.0f));
            renderer->Texture(bg->texture);
            renderer->Fill(bg->color);
            renderer->DrawQuad(Math::Vec3(width, height, 1.0f));
        }
    }

    void BackgroundLayer::OnDetach() {
        Layer::OnDetach();
    }

    void BackgroundLayer::HandleEvent(Event* event) {
        Layer::HandleEvent(event);
         DispatchEvent<Events::WindowResizedEvent>(event, [this](Events::WindowResizedEvent* window_event){
            width = window_event->GetWidth();
            height = window_event->GetHeight();
            LA_CORE_INFO("Background layer got window resized event!, {} {}", width, height);
         });
    }
}
