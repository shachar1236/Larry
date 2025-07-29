#pragma once

#include "Layers/ILayer.h"
#include <vector>

namespace Larry {
    class LayerStack {
        private:
            int last_id = 0;
        public:
            std::vector<Ref<ILayer>> layers;

            LayerStack() {}
            ~LayerStack() {}

            void AttachLayer(const Ref<ILayer>& layer);
            void DetachLayer(const std::string& name);
            void DetachLayer(const int& id);
            Ref<ILayer> GetLayer(const std::string& name);
            Ref<ILayer> GetLayer(const int& id);
            
            // void UpdateLayers() - run OnUpdate() on every layer from start to finish
            void UpdateLayers(const double& deltaTime);
            // void PassEventToLayers() - pass Event to every layer from finish to start until someone handels it 
            void PassEventToLayers(Event* event);
            // void PassEventToLayers() - pass Event to every layer from start to finish until someone handels it 
            void PassEventToLayersFromStartToFinish(Event* event);
    };
}
