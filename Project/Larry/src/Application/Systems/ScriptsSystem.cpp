#include "ScriptsSystem.h"
#include "Scripts/Scripts.h"

namespace Larry {

    void ScriptsSystem::OnCreate() {
        
    }

    
    void ScriptsSystem::OnUpdate(double deltaTime) {
        world->System<Scripts::ScriptsComponent>([deltaTime](Scripts::ScriptsComponent& scripts){
            for (auto& script : scripts) {
                script->OnUpdate(deltaTime);
            }
        });
    }

    void ScriptsSystem::OnDelete() {

    }

    void ScriptsSystem::HandleEvent(const Ref<Event>& event) {
        world->System<Scripts::ScriptsComponent>([event](Scripts::ScriptsComponent& scripts){
            for (int i = 0; i < scripts.size() && !event->Handeled; i++) {
                scripts[i]->HandleEvent(event);
            }
        });
    }
}
