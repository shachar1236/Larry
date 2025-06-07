#include "ECS_pch.h"
#include "common.h"
#include "Application/Systems/ScriptsSystem.h"
#include "Entity.hpp"
#include "EventSystem.h"
#include "InputEvents.h"
#include "Utils/Log.h"
#include "Scripts/Scripts.h"

namespace Larry {
    using namespace Scripts;

    void ScriptsSystem::OnCreate() {
        
    }

    
    void ScriptsSystem::OnUpdate(double deltaTime) {
        ECS::Internal::AnyQueue* any_queue = iworld->InitAnyQueue();
        ECS::Internal::TypeQueue* type_queue = iworld->InitTypeQueue();
        for (auto&& hash_code : scripts_types) {
            any_queue->Clear();
            type_queue->Clear();

            type_queue->push_back(hash_code);

            iworld->System(*type_queue, *any_queue, [deltaTime](ECS_Entity entity, ECS::Internal::AnyQueue& components, bool* stop){
                ECS_Any res = components.Pop();
                Ref<Script>* script_ptr = (Ref<Script>*)res.value;

                (*script_ptr)->OnUpdate(entity, deltaTime);
            });

        }

    }

    void ScriptsSystem::OnDelete() {

    }

    void ScriptsSystem::HandleEvent(const Ref<Event>& event) {
        ECS::Internal::AnyQueue* any_queue = iworld->InitAnyQueue();
        ECS::Internal::TypeQueue* type_queue = iworld->InitTypeQueue();

        for (auto&& hash_code : scripts_types) {
            any_queue->Clear();
            type_queue->Clear();

            type_queue->push_back(hash_code);

            iworld->System(*type_queue, *any_queue, [event](ECS_Entity entity, ECS::Internal::AnyQueue& components, bool* stop){
                ECS_Any res = components.Pop();
                Ref<Script>* script_ptr = (Ref<Script>*)res.value;

                DispatchEvent<Events::KeyPressedEvent>(event, [entity, script_ptr](const Ref<Event>& e){
                    (*script_ptr)->ButtonJustPressed(entity, (Events::KeyPressedEvent*)e.get());
                });
                DispatchEvent<Events::KeyReleasedEvent>(event, [entity, script_ptr](const Ref<Event>& e){
                    (*script_ptr)->ButtonReleased(entity, (Events::KeyReleasedEvent*)e.get());
                });
                DispatchEvent<Events::MousePressedEvent>(event, [entity, script_ptr](const Ref<Event>& e){
                    (*script_ptr)->MouseJustPressed(entity, (Events::MousePressedEvent*)e.get());
                });
                DispatchEvent<Events::MouseScrolledEvent>(event, [entity, script_ptr](const Ref<Event>& e){
                    (*script_ptr)->MouseScrolled(entity, (Events::MouseScrolledEvent*)e.get());
                });
                if (!event->Handeled) {
                    (*script_ptr)->HandleEvent(entity, event);
                }
            });

        }

    }
}
