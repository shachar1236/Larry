#pragma once

#include "ECS/Internal/TypesBitmap.hpp"
#include "ECS_C.h"
#include <cstdint>

namespace Larry::ECS::Internal {
    using Entity = ECS_Entity;

    int32_t GetEntityIdentifier(Entity entity) {
        return entity;
    }

    int32_t GetEntityVersion(Entity entity) {
        return entity >> 32;
    }

    Entity IncreaseEntityVersion(Entity entity) {
        int64_t entity_version = GetEntityVersion(entity) + 1;
        return entity | (entity_version << 32);
    }
}
