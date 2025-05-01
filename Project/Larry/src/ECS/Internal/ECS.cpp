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

void ECS_RegisterType(ECS_World world, ECS_TypeHashCode type, int type_size, void(*destructor)(const void*)) {
    World* real_world = (World*)world;
    real_world->GetTypeManager()->RegisterType(type, type_size, destructor);
}

ECS_Entity ECS_CreateEntity(ECS_World world) {
    World* real_world = (World*)world;
    return real_world->CreateEntity();
}
