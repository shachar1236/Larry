#include "Event.h"
#include "LarryMemory.h"
#include "Layer.h"

namespace Larry {
    void Layer::OnUpdate(const double& deltaTime) {
        for (int i = 0; i < systems.size(); i++) {
            systems[i]->OnUpdate(*world, deltaTime);
        }
    }

    void Layer::OnDetach() {
        for (int i = 0; i < systems.size(); i++) {
            systems[i]->OnDelete(*world);
        }
    }

    void Layer::HandleEvent(const Ref<Event>& event) {
        for (int i = 0; i < systems.size() && !event->Handeled; i++) {
            systems[i]->HandleEvent(*world, event);
        }
    };


    void Layer::AddSystem(const Ref<ISystem>& system) {
        systems.push_back(system);
        system->OnCreate(*world);
    }
}
