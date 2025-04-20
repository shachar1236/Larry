#include "Application/Components/Projection.h"
#include "ECS.h"
#include "Entity.hpp"
#include "Event.h"
#include "ILayer.h"
#include "LarryMemory.h"
#include "Layer.h"

namespace Larry {
    Layer::Layer(const std::string& name_, const Ref<ECS::World>& world_) : ILayer(name_), world(world_) {
        renderer = Renderer::GetRenderer();
    }

    void Layer::OnUpdate(const double& deltaTime) {
        for (int i = 0; i < systems.size(); i++) {
            systems[i]->OnUpdate(deltaTime);
        }

        world->AdvancedSystem<Projection>([this](ECS::Entity _, ECS::BreakFunction brk, Projection& proj){
            if (proj.projection_layers.contains(id)) {
                this->renderer->ChangeProjection(proj.projection);
                brk();
            }
        });
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
