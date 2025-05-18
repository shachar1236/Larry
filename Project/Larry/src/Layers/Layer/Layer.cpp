#include "ECS_pch.h"
#include "common.h"
#include "Application/Components/Projection.h"
#include "Application/Components/Camera.h"
#include "ECS.h"
#include "Entity.hpp"
#include "Event.h"
#include "ILayer.h"
#include "LarryMemory.h"
#include "Log.h"
#include "Layer.h"

namespace Larry {
    Layer::Layer(const std::string& name_, const Ref<ECS::World>& world_) : ILayer(name_), world(world_) {
        renderer = Renderer::GetRenderer();
    }

    void Layer::OnAttach() {
        LA_CORE_INFO("Layer {} id {}", name, id);
    }

    void Layer::OnUpdate(const double& deltaTime) {
        world->AdvancedSystem<Projection>([this](ECS::Entity _, bool* stop, Projection& proj){
            if (proj.projection_layers.contains(id)) {
                this->renderer->ChangeProjection(proj.projection);
                *stop = true;
            }
        });

        world->AdvancedSystem<Camera>([this](ECS::Entity _, bool* stop, Camera& camera){
            if (camera.view_layers.contains(id)) {
                /* LA_CORE_WARN("Found corresponding layer"); */
                this->renderer->ChangeView(camera.view);
                *stop = true;
            }
        });

        for (int i = 0; i < systems.size(); i++) {
            systems[i]->OnUpdate(deltaTime);
        }

    }

    void Layer::OnDetach() {
        for (int i = 0; i < systems.size(); i++) {
            systems[i]->OnDelete();
        }
    }

    void Layer::HandleEvent(const Ref<Event>& event) {
        for (int i = 0; i < systems.size() && !event->Handeled; i++) {
            systems[i]->HandleEvent(event);
        }
    };


    void Layer::AddSystem(const Ref<ISystem>& system) {
        systems.push_back(system);
        system->OnCreate();
    }
}
