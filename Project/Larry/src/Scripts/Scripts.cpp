// !!!!! this file should not be used unless you dont have GameScripts compiled
#include "common.h"
#include "Utils/LarryMemory.h"
#include "Scripts/Scripts.h"
#include <string>

namespace Larry::Scripts {
    std::unordered_map<ECS_TypeHashCode, char*> Script::scriptType_to_layerName;
    std::unordered_map<std::string, Script::CreateScriptFunction> Script::scriptName_to_CreateFunction;

    void Script::Init(const Ref<ECS::World>& world) {

    };

    void Script::RegisterScripts(LayerStack&) {

    }
}
