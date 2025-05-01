#include "ECS_C.h"
#include "ECS/Internal/TypesBitmap.hpp"
#include "ECS/Internal/World.hpp"

using namespace Larry::ECS::Internal;

std::bitset<256> TypesBitmap::first32mask = 0;

void TypesBitmap::Init() {
    for (int i = 0; i < 32; i++) {
        TypesBitmap::first32mask.set(i);
    }
}

void ECS_Init() {
    TypesBitmap::Init();
}

void* ECS_CreateWorld() {
    return (void*)(new World());
}

ECS_AnyQueue ECS_InitAnyQueue(ECS_World world) {
    World
}
