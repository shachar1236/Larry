#include "common.h"
#include "RenderQuad.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "Log.h"
#include "Renderer.h"

namespace Larry {

    void RenderQuad::OnCreate() {
        renderer = *world->GetSingelton<Renderer*>().value();
    }

    void RenderQuad::OnUpdate(double deltaTime) {
        world->System<Transform, Quad>([this](Transform& transform, Quad& quad){
            renderer->Translate(transform.translation);
            renderer->Rotate(transform.rotation_size, transform.rotation_axis);
            renderer->Scale(transform.scale);
            renderer->Fill(quad.color);
            if (quad.texture != nullptr) {
                renderer->Texture(quad.texture);
            }
            renderer->DrawQuad(quad.dimentions);
        });
    }

    void RenderQuad::OnDelete() {

    }

    void RenderQuad::HandleEvent(const Ref<Event>& event) {

    }
}
