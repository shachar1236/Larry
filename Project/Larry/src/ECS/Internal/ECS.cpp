#include "common.h"
#include "ECS/ECS_C.h"
#include "ECS/Internal/TypesBitmap.hpp"
#include "ECS/Internal/World.hpp"
#include "ECS/Internal/Queues.h"

using namespace Larry::ECS::Internal;

void ECS_Init() {
    TypesBitmap::Init();
}

void* ECS_CreateWorld() {
    return (void*)(new World());
}

void ECS_RegisterType(ECS_World world_, ECS_TypeHashCode type, int type_size, void(*destructor)(const void*)) {
    World* world = (World*)world_;
    world->GetTypeManager()->RegisterType(type, type_size, destructor);
}

ECS_Entity ECS_CreateEntity(ECS_World world_) {
    World* world = (World*)world_;
    return world->CreateEntity();
}

bool ECS_IsEntityAlive(ECS_World world_, ECS_Entity entity) {
    World* world = (World*)world_;
    return world->IsEntityAlive(entity);
}

void ECS_KillEntity(ECS_World world_, ECS_Entity entity) {
    World* world = (World*)world_;
    world->KillEntity(entity);
}

ECS_AnyQueue ECS_InitAnyQueue(ECS_World world_) {
    World* world = (World*)world_;
    return world->InitAnyQueue();
}

void ECS_PushToAnyQueue(ECS_AnyQueue queue_, ECS_Any any) {
    AnyQueue* queue = (AnyQueue*)queue;
    queue->elements.push_back(any);
}

ECS_Any ECS_PopFromAnyQueue(ECS_AnyQueue queue_) {
    AnyQueue* queue = (AnyQueue*)queue;
    return queue->Pop();
}

void ECS_DoneWithAnyQueue(ECS_World world_, ECS_AnyQueue queue_) {
    World* world = (World*)world_;
    AnyQueue* queue = (AnyQueue*)queue;
    world->DoneWithAnyQueue(queue);
}

ECS_TypeQueue ECS_InitTypeQueue(ECS_World world_) {
    World* world = (World*)world_;
    return world->InitTypeQueue();
}

void ECS_PushToTypeQueue(ECS_TypeQueue queue_, ECS_TypeHashCode code) {
    TypeQueue* queue = (TypeQueue*)queue_;
    queue->push_back(code);
}

void ECS_DoneWithTypeQueue(ECS_World world_, ECS_TypeQueue queue_) {
    World* world = (World*)world_;
    TypeQueue* queue = (TypeQueue*)queue_;
    world->DoneWithTypeQueue(queue);
}

bool ECS_InsertComponents(ECS_World world_, ECS_Entity entity, ECS_TypeQueue types_, ECS_AnyQueue resultQueue_) {
    World* world = (World*)world_;
    TypeQueue* types = (TypeQueue*)types;
    AnyQueue* resultQueue = (AnyQueue*)resultQueue_;

    return world->InsertComponents(entity, *types, *resultQueue);
}

void ECS_SetComponents(ECS_World world_, ECS_Entity entity, ECS_TypeQueue types_, ECS_AnyQueue resultQueue_) {
    World* world = (World*)world_;
    TypeQueue* types = (TypeQueue*)types;
    AnyQueue* resultQueue = (AnyQueue*)resultQueue_;
    
    world->SetComponents(entity, *types, *resultQueue);
}

ECS_Any ECS_GetComponent(ECS_World world_, ECS_Entity entity, ECS_TypeHashCode code) {
    World* world = (World*)world_;

    return world->GetComponent(entity, code).value_or(ECS_Any{ NULL, 0 });
}

void ECS_DeleteComponent(ECS_World world_, ECS_Entity entity, ECS_TypeHashCode code) {
    World* world = (World*)world_;

    world->DeleteComponent(entity, code);
}

void* ECS_CreateSingelton(ECS_World world_, ECS_TypeHashCode singelton_hash) {
    World* world = (World*)world_;
    
    return world->CreateSingelton(singelton_hash);
}

void* ECS_GetSingelton(ECS_World world_, ECS_TypeHashCode code) {
    World* world = (World*)world_;
    
    return world->GetSingelton(code).value_or((void*)NULL);
}


void ECS_System(ECS_World world_, ECS_TypeQueue components_types_, ECS_AnyQueue system_components_queue_, void(*SystemFunc)(ECS_Entity, ECS_AnyQueue, bool*)) {
    World* world = (World*)world_;
    TypeQueue* components_types = (TypeQueue*)components_types_;
    AnyQueue* system_components_queue = (AnyQueue*)system_components_queue_;

    world->System(*components_types, *system_components_queue, [SystemFunc](ECS_Entity entity, AnyQueue& components, bool* stop){
        SystemFunc(entity, &components, stop);
    });
}




