// !!!!! this file should not be used unless you dont have GameScripts compiled
#include "Utils/LarryMemory.h"
#include "Scripts/Scripts.h"
#include <string>

namespace Larry::Scripts {

    void Script::Init(const Ref<ECS::World>& world) {

    };

    Ref<Script> Script::GetNewInstanceOfScript(const std::string& name, const Ref<ECS::World>& world) {
        return nullptr;
    };
}
