#pragma once
#include "ECS/ECS_C.h"

namespace Larry::ECS {
    using Entity = ECS_Entity;
    using Any = ECS_Any;

    void Init() {
        ECS_Init();
    }
}
