#include "Application/Systems/ScriptsSystem.h"
#include "Entity.hpp"
#include "EventSystem.h"
#include "InputEvents.h"
#include "Utils/Log.h"
#include "Scripts/Scripts.h"

namespace Larry {

    void ScriptsSystem::OnCreate() {
        
    }

    
    void ScriptsSystem::OnUpdate(double deltaTime) {
        world->AdvancedSystem<Scripts::ScriptsComponent>([deltaTime](const ECS::Entity& entity, bool* stop, Scripts::ScriptsComponent& scripts){
            for (auto& script : scripts) {
                script->OnUpdate(entity, deltaTime);
            }
        });
    }

    void ScriptsSystem::OnDelete() {

    }

    void ScriptsSystem::HandleEvent(const Ref<Event>& event) {
        world->AdvancedSystem<Scripts::ScriptsComponent>([event](const ECS::Entity& entity, bool* stop, Scripts::ScriptsComponent& scripts) {

            for (int i = 0; i < scripts.size() && !event->Handeled; i++) {
                DispatchEvent<Events::KeyPressedEvent>(event, [entity, scripts, i](const Ref<Event>& e){
                    scripts[i]->ButtonJustPressed(entity, (Events::KeyPressedEvent*)e.get());
                });
                DispatchEvent<Events::KeyReleasedEvent>(event, [entity, scripts, i](const Ref<Event>& e){
                    scripts[i]->ButtonReleased(entity, (Events::KeyReleasedEvent*)e.get());
                });
                DispatchEvent<Events::MousePressedEvent>(event, [entity, scripts, i](const Ref<Event>& e){
                    scripts[i]->MouseJustPressed(entity, (Events::MousePressedEvent*)e.get());
                });
                DispatchEvent<Events::MouseScrolledEvent>(event, [entity, scripts, i](const Ref<Event>& e){
                    scripts[i]->MouseScrolled(entity, (Events::MouseScrolledEvent*)e.get());
                });
                if (!event->Handeled) {
                    scripts[i]->HandleEvent(entity, event);
                }
            }

        });
    }
}
