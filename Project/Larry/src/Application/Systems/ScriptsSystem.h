#pragma once

#include "EventSystem/Event.h"
#include "Renderer.h"
#include "Systems/System.h"
#include "ECS/CPPApi/World.hpp"
#include "ECS/Internal/World.hpp"
#include <vector>

namespace Larry {
    class ScriptsSystem : public ISystem {
        private:
            ECS::Internal::World* iworld;

            std::vector<ECS_TypeHashCode> scripts_types;
        public:

            ScriptsSystem(ECS::World* world_) : ISystem(world_) {
                iworld = world_->GetInternalWorld();
            }
            ~ScriptsSystem() {}

            inline void AddScriptType(ECS_TypeHashCode hash) {
                scripts_types.push_back(hash);
            }

            virtual void OnCreate();
            virtual void OnUpdate(double deltaTime);
            virtual void OnDelete();

            virtual void HandleEvent(Event* event);
    };
}
