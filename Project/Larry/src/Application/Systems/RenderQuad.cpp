#include "RenderQuad.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "Log.h"
#include "Renderer.h"

namespace Larry {

    void RenderQuad::OnCreate(ECS::World& world) {
        renderer = *world.GetSingelton<Renderer*>().value();
    }

    void RenderQuad::OnUpdate(ECS::World& world, const double& deltaTime) {
        world.System<Transform, Quad>([this](Transform& transform, Quad& quad){
            renderer->Translate(transform.translation);
            renderer->Rotate(transform.rotation_size, transform.rotation_axis);
            renderer->Scale(transform.scale);
            renderer->Fill(quad.color);
            renderer->DrawQuad(quad.dimentions);
        });
    }

    void RenderQuad::OnDelete(ECS::World&) {

    }

    void RenderQuad::HandleEvent(ECS::World&, const Ref<Event>& event) {

    }
}
