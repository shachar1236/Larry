#pragma once

#include "EventSystem/Event.h"
#include "Utils/LarryMemory.h"
#include <string>

namespace Larry {
    class ILayer {
        protected:
            std::string name;
            int id;
        public:
            ILayer(const std::string& name_) : name(name_) {}
            virtual ~ILayer() {};

            virtual void OnAttach() = 0;
            virtual void OnUpdate(const double& deltaTime) = 0;
            virtual void OnDetach() = 0;
            virtual void HandleEvent(Event* event) = 0;

            std::string GetName() { return name; }
            void SetId(const int& id_) { id = id_; }
            int GetId() { return id; }
    };
}
