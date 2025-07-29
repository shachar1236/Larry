#pragma once

#include "gl.h"
#include "EventSystem/Event.h"
#include "Utils/LarryMemory.h"
#include "Application/Systems/System.h"
#include "Renderer.h"
#include "World.hpp"
#include "Layers/ILayer.h"
#include <string>
#include <vector>

namespace Larry {
    class Layer : public ILayer {
        protected:
            std::vector<Ref<ISystem>> systems;
            ECS::World* world;
            Renderer* renderer;
        public:
            Layer(const std::string& name_, ECS::World* world_);
            virtual ~Layer() {};

            virtual void OnAttach();
            virtual void OnUpdate(const double& deltaTime);
            virtual void OnDetach();
            virtual void HandleEvent(Event* event);

            void AddSystem(const Ref<ISystem>& system);
            std::string GetName() { return name; }
            void SetId(const int& id_) { id = id_; }
            int GetId() { return id; }
    };
}
