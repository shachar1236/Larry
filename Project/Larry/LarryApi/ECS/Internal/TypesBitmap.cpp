#include "common.h"
#include "ECS/Internal/TypesBitmap.hpp"

namespace Larry::ECS::Internal {
    std::bitset<256> TypesBitmap::first32mask = 0;

    void TypesBitmap::Init() {
        for (int i = 0; i < 32; i++) {
            TypesBitmap::first32mask.set(i);
        }
    }
}
