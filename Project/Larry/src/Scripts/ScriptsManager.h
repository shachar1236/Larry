#pragma once

#include "ECS/CPPApi/ECS.h"
#include "ECS/CPPApi/World.hpp"
#include "Utils/LarryMemory.h"
#include "Layers/LayerStack.h"
#include <string>

namespace Larry::Scripts {

    void RegisterScripts(const Ref<ECS::World>& world, LayerStack& layerStack);
    void AddScriptToEntity(ECS::Entity entity, const std::string& scriptName, const Ref<ECS::World>& world);

}
