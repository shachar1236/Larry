#include "Scripts/ScriptsManager.h"
#include "ECS/Internal/World.hpp"
#include "Utils/LarryMemory.h"
#include "Layers/Layer/Layer.h"
#include "Utils/Log.h"
#include "ECS/Internal/Queues.h"
#include "Scripts/Scripts.h"
#include "Systems/ScriptsSystem.h"

namespace Larry::Scripts {

    void RegisterScripts(const Ref<ECS::World>& world, LayerStack& layerStack) {
        LA_CORE_INFO("Registering c++ scripts...");
        ECS::Internal::World* iworld = world->GetInternalWorld();

        std::unordered_map<std::string, Ref<ScriptsSystem>> systems;
        
        for (auto&& [name, details] : Script::scriptName_to_detailes) {
            iworld->GetTypeManager()->RegisterType(details.hash_code, details.size, details.destractor_function);

            Ref<ScriptsSystem> script_system;

            auto system_it = systems.find(details.layer_name);
            if (system_it == systems.end()) {
                script_system = CreateRef<ScriptsSystem>(world);
                systems[details.layer_name] = script_system;
            } else {
                script_system = system_it->second;
            }

            script_system->AddScriptType(details.hash_code);
        }

        for (auto&& [layerName, system] : systems) {
            static_cast<Layer*>(layerStack.GetLayer(layerName).get())->AddSystem(system);
        }

        LA_CORE_INFO("Done Registering c++ scripts!");
    }

    void AddScriptToEntity(ECS::Entity entity, const std::string& scriptName, const Ref<ECS::World>& world) {
        LA_CORE_INFO("Adding script {} to entity {}", scriptName, entity);

        ECS::Internal::World* iworld = world->GetInternalWorld();
        auto details = Script::scriptName_to_detailes[scriptName];
        ECS::Internal::AnyQueue* result_queue = iworld->InitAnyQueue();

        ECS::Internal::TypeQueue* type_queue = iworld->InitTypeQueue();
        type_queue->push_back(details.hash_code);

        iworld->InsertComponents(entity, *type_queue, *result_queue);
        
        ECS_Any res = result_queue->Pop();
        Script* script_ptr = (Script*)res.value;

        details.create_function(res.value, world);
        // *script_ptr = details.create_function(world);
        script_ptr->OnCreate(entity);
    }
}
