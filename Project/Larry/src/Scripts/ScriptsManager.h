#pragma once

#include "gl.h"
#include "ECS/CPPApi/ECS.h"
#include "ECS/CPPApi/World.hpp"
#include "Utils/LarryMemory.h"
#include "Layers/LayerStack.h"
#include <string>

namespace Larry::Scripts {

    void RegisterScripts(ECS::World* world, LayerStack& layerStack);
    void AddScriptToEntity(ECS::Entity entity, const std::string& scriptName, ECS::World* world);

}
