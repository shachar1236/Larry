#include "ECS_pch.h"
#include "Math/Math.h"
#include "common.h"
#include "RenderQuad.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "Utils/Log.h"
#include "Renderer.h"

namespace Larry {

    void RenderQuad::OnCreate() {
        renderer = *world->GetSingelton<Renderer*>();
    }

    void RenderQuad::OnUpdate(double deltaTime) {
        world->System<Transform, Quad>([this](Transform& transform, Quad& quad){
            // LA_CORE_TRACE("In render quad");
            Math::Vec3 translation = transform._realTranslation;
            renderer->Translate(transform._realTranslation);
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
