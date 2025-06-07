#include "LarryApi.h"
#include "TestScript.h"
#include <string>

namespace Larry::Scripts {

    std::unordered_map<std::string, Script::scriptDetails> Script::scriptName_to_detailes;

    void Script::Init(const Ref<ECS::World>& world) {

    };
}
