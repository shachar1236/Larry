// !!!!! this file should not be used unless you dont have GameScripts compiled
#include "common.h"
#include "Utils/LarryMemory.h"
#include "Scripts/Scripts.h"
#include <string>

namespace Larry::Scripts {
    std::unordered_map<std::string, Script::scriptDetails> Script::scriptName_to_detailes;

    void Script::Init(const Ref<ECS::World>& world) {

    };
}
