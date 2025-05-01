#pragma once

#include <cstdint>

typedef unsigned long ECS_TypeHashCode;

typedef struct {
    void* value;
    ECS_TypeHashCode type;
} ECS_Any;

typedef void* ECS_World;
typedef int64_t ECS_Entity;
typedef void* ECS_AnyQueue;
typedef void* ECS_TypeQueue;

void ECS_Init();

void* ECS_CreateWorld();

void ECS_RegisterType(ECS_World, ECS_TypeHashCode type, int type_size, void(*destructor)(const void*));

ECS_Entity ECS_CreateEntity(ECS_World);
bool ECS_IsEntityAlive(ECS_Entity);
ECS_Entity ECS_KillEntity(ECS_World);

// gives you a queue to work with
ECS_AnyQueue ECS_InitAnyQueue(ECS_World);
// pushes a value to queue
void ECS_PushToAnyQueue(ECS_AnyQueue, ECS_Any);
// pops a value to queue
ECS_Any ECS_PopFromAnyQueue(ECS_AnyQueue queue);
// when you are done using the queue call this function and the world will take ownership of the state
void ECS_DoneWithAnyQueue(ECS_World, ECS_AnyQueue queue);

// gives you a state to work with
ECS_TypeQueue ECS_InitTypeQueue(ECS_World);
// pushes a value to state
void ECS_PushToTypeQueue(ECS_TypeQueue, ECS_TypeHashCode);
// when you are done using the state call this function and the world will take ownership of the state
void ECS_DoneWithTypeQueue(ECS_World, ECS_TypeQueue queue);

void ECS_InsertComponents(ECS_World, ECS_Entity, ECS_AnyQueue);
void ECS_SetComponents(ECS_World, ECS_Entity, ECS_AnyQueue);
ECS_Any ECS_GetComponent(ECS_World, ECS_Entity, ECS_TypeHashCode);
void ECS_DeleteComponent(ECS_World, ECS_Entity, ECS_TypeHashCode);

void ECS_System(ECS_World, ECS_TypeQueue components_types, ECS_AnyQueue system_components_queue, void(*SystemFunc)(ECS_Entity, ECS_AnyQueue components, bool* stop));

void* ECS_CreateSingelton(ECS_World, ECS_Any); // returns a pointer to the singelton
void* ECS_GetSingelton(ECS_World, ECS_TypeHashCode);
