#pragma once

#include "Event.h"
#include "LarryMemory.h"
#include <string>

namespace Larry {
    class Layer {
        protected:
            std::string name;
            int id;
        public:
            Layer(const std::string& name_) : name(name_) {}
            virtual ~Layer() {};

            virtual void OnAttach() = 0;
            virtual void OnUpdate(const double& deltaTime) = 0;
            virtual void OnDetach() = 0;
            virtual void HandleEvent(const Ref<Event>& event) = 0;

            std::string GetName() { return name; }
            void SetId(const int& id_) { id = id_; }
            int GetId() { return id; }
    };
}
