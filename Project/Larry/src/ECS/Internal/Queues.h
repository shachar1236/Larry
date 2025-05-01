#pragma once

#include "ECS_C.h"
#include "Internal/TypesBitmap.hpp"
#include <vector>

namespace Larry::ECS::Internal {

    struct AnyQueue {
        std::vector<ECS_Any> elements;
        int index = 0;

        void Clear() {
            elements.clear();
            index = 0;
        }

        ECS_Any Pop() {
            ECS_Any res = elements[index];
            index++;
            return res;
        }
    };
    using TypeQueue = std::vector<ECS_TypeHashCode>;

}

