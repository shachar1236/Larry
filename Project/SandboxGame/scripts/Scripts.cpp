#include "TestScript.h"
#include "Utils/LarryMemory.h"
#include "Scripts/Scripts.h"
#include <string>

namespace Larry::Scripts {

    void Script::Init() {

    };

    Ref<Script> Script::GetNewInstanceOfScript(const std::string& name) {
        return CreateRef<TestScript>();
    };
}
