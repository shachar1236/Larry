#include "LayerStack.h"

namespace Larry {
    
    void LayerStack::AttachLayer(const Ref<Layer>& layer) {
        layer->SetId(last_id);
        last_id++;
        layers.push_back(layer);
        layer->OnAttach();
    }

    void LayerStack::DetachLayer(const std::string& name) {
        for (int i = 0; i < layers.size(); i++) {
            if (layers[i]->GetName() == name) {
                layers[i]->OnDetach();
                layers.erase(layers.begin() + i);
                break;
            }
        }
    }

    void LayerStack::DetachLayer(const int& id) {
        for (int i = 0; i < layers.size(); i++) {
            if (layers[i]->GetId() == id) {
                layers[i]->OnDetach();
                layers.erase(layers.begin() + i);
                break;
            }
        }
    }

    Ref<Layer> LayerStack::GetLayer(const std::string& name) {
        for (int i = 0; i < layers.size(); i++) {
            if (layers[i]->GetName() == name) {
                return layers[i];
            }
        }
        return nullptr;
    }

    Ref<Layer> LayerStack::GetLayer(const int& id) {
        for (int i = 0; i < layers.size(); i++) {
            if (layers[i]->GetId() == id) {
                return layers[i];
            }
        }
        return nullptr;
    }

    void LayerStack::UpdateLayers(const double& deltaTime) {
        for (auto layer : layers) {
            layer->OnUpdate(deltaTime);
        }
    }

    void LayerStack::PassEventToLayers(const Ref<Event>& event) {
        for (int i = layers.size() - 1; i > -1 && !event->Handeled; i--) {
            layers[i]->HandleEvent(event);
        }
    }

    void LayerStack::PassEventToLayersFromStartToFinish(const Ref<Event>& event) {
        for (int i = 0; i < layers.size() && !event->Handeled; i++) {
            layers[i]->HandleEvent(event);
        }
    }
}
