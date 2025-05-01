#pragma once

#include "ECS_C.h"
#include "Internal/TypesBitmap.hpp"
#include <vector>

namespace Larry::ECS::Internal {

    using AnyQueue = std::vector<ECS_Any>;
    using TypeQueue = std::vector<ECS_TypeHashCode>;

}

