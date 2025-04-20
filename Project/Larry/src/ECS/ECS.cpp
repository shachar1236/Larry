#include "ECS.h"
#include "ECS/TypesBitmap.hpp"

namespace Larry::ECS {
    std::bitset<256> TypesBitmap::first32mask = 0;

    void Init() {
        TypesBitmap::Init();
    }

    void TypesBitmap::Init() {
        for (int i = 0; i < 32; i++) {
            TypesBitmap::first32mask.set(i);
        }
    }
}
