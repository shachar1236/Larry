#pragma once

#include "ECS/ECS_C.h"
#include <cstdint>

namespace Larry::ECS::Internal {
    using Entity = ECS_Entity;

    inline int32_t GetEntityIdentifier(Entity entity) {
        return entity;
    }

    inline int32_t GetEntityVersion(Entity entity) {
        return entity >> 32;
    }

    inline Entity IncreaseEntityVersion(Entity entity) {
        int64_t entity_version = GetEntityVersion(entity) + 1;
        return entity | (entity_version << 32);
    }
}
