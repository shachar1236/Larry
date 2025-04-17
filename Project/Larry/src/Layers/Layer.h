#pragma once

#include "Event.h"
#include "LarryMemory.h"
#include "Application/Systems/System.h"
#include "World.hpp"
#include <string>
#include <vector>

namespace Larry {
    class Layer {
        protected:
            std::string name;
            int id;

            std::vector<Ref<ISystem>> systems;
            Ref<ECS::World> world;
        public:
            Layer(const std::string& name_, const Ref<ECS::World>& world_) : name(name_), world(world_) {}
            virtual ~Layer() {};

            virtual void OnAttach() = 0;
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(const Ref<Event>& event);

            void AddSystem(const Ref<ISystem>& system);
            std::string GetName() { return name; }
            void SetId(const int& id_) { id = id_; }
            int GetId() { return id; }
    };
}
