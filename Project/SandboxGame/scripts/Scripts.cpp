#include "LarryApi.h"
#include "TestScript.h"
#include <string>

namespace Larry::Scripts {

    void Script::Init(const Ref<ECS::World>& world) {

    };

    Ref<Script> Script::GetNewInstanceOfScript(const std::string& name, const Ref<ECS::World>& world) {
        return CreateRef<TestScript>(world);
    };
}
