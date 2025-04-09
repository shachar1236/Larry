#pragma once

#include "Layers/Layer.h"
#include <vector>

namespace Larry {
    class LayerStack {
        private:
            int last_id = 0;
        public:
            std::vector<Ref<Layer>> layers;

            LayerStack() {}
            ~LayerStack() {}

            void AttachLayer(const Ref<Layer>& layer);
            void DetachLayer(const std::string& name);
            void DetachLayer(const int& id);
            Ref<Layer> GetLayer(const std::string& name);
            Ref<Layer> GetLayer(const int& id);
            
            // void UpdateLayers() - run OnUpdate() on every layer from start to finish
            void UpdateLayers(const double& deltaTime);
            // void PassEventToLayers() - pass Event to every layer from finish to start until someone handels it 
            void PassEventToLayers(const Ref<Event>& event);
            // void PassEventToLayers() - pass Event to every layer from start to finish until someone handels it 
            void PassEventToLayersFromStartToFinish(const Ref<Event>& event);
    };
}
