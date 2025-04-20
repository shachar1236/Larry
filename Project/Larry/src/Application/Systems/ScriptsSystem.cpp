#include "Application/Systems/ScriptsSystem.h"
#include "ECS/ECS.h"
#include "Entity.hpp"
#include "Utils/Log.h"
#include "Scripts/Scripts.h"

namespace Larry {

    void ScriptsSystem::OnCreate() {
        
    }

    
    void ScriptsSystem::OnUpdate(double deltaTime) {
        world->AdvancedSystem<Scripts::ScriptsComponent>([deltaTime](const ECS::Entity& entity, ECS::BreakFunction& brk, Scripts::ScriptsComponent& scripts){
            for (auto& script : scripts) {
                script->OnUpdate(entity, deltaTime);
            }
        });
    }

    void ScriptsSystem::OnDelete() {

    }

    void ScriptsSystem::HandleEvent(const Ref<Event>& event) {
        world->AdvancedSystem<Scripts::ScriptsComponent>([event](const ECS::Entity& entity, ECS::BreakFunction& brk, Scripts::ScriptsComponent& scripts){
            for (int i = 0; i < scripts.size() && !event->Handeled; i++) {
                scripts[i]->HandleEvent(entity, event);
            }
        });
    }
}
