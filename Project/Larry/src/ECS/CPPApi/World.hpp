#pragma once
#include "ECS/CPPApi/ECS.h"
#include "ECS/ECS_C.h"
#include "ECS/Internal/Queues.h"
#include "ECS/Internal/TypeManager.hpp"
#include "ECS/Internal/World.hpp"
#include "Log.h"
#include <optional>

#define TypeHash(T) typeid(T).hash_code()
#define RegisterTypes(Ts) (ECS_RegisterType(&world, TypeHash(Ts), sizeof(Ts), DESTRUCTOR_LAMBDA(Ts)), ...);
#define ForEachType(x) (x, ...)
// #define CallFWithAnyQueue(queue) F((Types&)(*(Types*)(queue->Pop().value))...)

namespace Larry::ECS {
class World {
private:
    Internal::World world;

public:
    World() { }

    ~World() { }

    Entity CreateEntity() { return ECS_CreateEntity(&world); }

    bool IsEntityAlive(Entity entity) { return world.IsEntityAlive(entity); }

    void KillEntity(Entity entity) { world.KillEntity(entity); }

    template <typename... Types, typename F> 
    bool InsertComponent(Entity entity, F set_callback)
    {
        RegisterTypes(Types);

        LA_CORE_ERROR("Hiiii");

        Internal::AnyQueue* resultQueue = world.InitAnyQueue();
        Internal::TypeQueue* types = world.InitTypeQueue();
        // ForEachType(LA_CORE_DEBUG("Type hash: {}", TypeHash(Types)));
        ForEachType(types->push_back(TypeHash(Types)));
        bool success = world.InsertComponents(entity, *types, *resultQueue);

        if (success) {
            resultQueue->InitPopBack();
            set_callback((Types&)(*(Types*)(resultQueue->PopBack().value))...);
        }

        world.DoneWithAnyQueue(resultQueue);
        world.DoneWithTypeQueue(types);

        return success;
    }

    template<typename T>
    std::optional<const T*> GetComponent(Entity entity)
    {
        ECS_RegisterType(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));
        std::optional<ECS_Any> res = world.GetComponent(entity, TypeHash(T));
        if (res.has_value()) {
            return (T*)res->value;
        }
        return std::nullopt;
    }

    template <typename... Types, typename F>
    bool SetComponents(Entity entity, F set_callback)
    {
        RegisterTypes(Types);

        Internal::AnyQueue* resultQueue = world.InitAnyQueue();
        Internal::TypeQueue* types = world.InitTypeQueue();
        ForEachType(types->push_back(TypeHash(Types)));
        bool succeded = world.SetComponents(entity, *types, *resultQueue);

        if (succeded) {
            resultQueue->InitPopBack();
            set_callback((Types&)(*(Types*)(resultQueue->PopBack().value))...);
        }

        world.DoneWithAnyQueue(resultQueue);
        world.DoneWithTypeQueue(types);

        return succeded;
    }

    template <typename T>
    void DeleteComponent(Entity entity)
    {
        ECS_RegisterType(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

        world.DeleteComponent(entity, TypeHash(T));
    }

    template <typename T, typename F>
    void CreateSingelton(const F& set_callback)
    {
        ECS_RegisterType(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

        T* res = (T*)world.CreateSingelton(TypeHash(T));
        set_callback(*res);
    }

    template <typename T> std::optional<T*> 
    GetSingelton()
    {
        ECS_RegisterType(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

        std::optional<void*> res = world.GetSingelton(TypeHash(T));
        if (res.has_value()) {
            return (T*)res.value();
        }
        return std::nullopt;
    }

    template <typename T, typename F> 
    void SetSingelton(const F& set_callback)
    {
        ECS_RegisterType(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

        std::optional<void*> res = world.GetSingelton(TypeHash(T));
        if (res.has_value()) {
            set_callback(*(T*)res.value());
        }
    }

    template <typename... Types, typename F> 
    void System(const F& callback)
    {
        RegisterTypes(Types);

        Internal::TypeQueue* types = world.InitTypeQueue();
        ForEachType(types->push_back(TypeHash(Types)));

        Internal::AnyQueue* system_components_queue = world.InitAnyQueue();

        world.System(*types, *system_components_queue,
            [callback](ECS_Entity entity, Internal::AnyQueue& components, bool* stop) {
                components.InitPopBack();
                callback((Types&)(*(Types*)(components.PopBack().value))...);
            });

        world.DoneWithAnyQueue(system_components_queue);
        world.DoneWithTypeQueue(types);
    }

    template <typename... Types, typename F> 
    void AdvancedSystem(const F& callback)
    {
        RegisterTypes(Types);

        Internal::TypeQueue* types = world.InitTypeQueue();
        ForEachType(types->push_back(TypeHash(Types)));

        Internal::AnyQueue* system_components_queue = world.InitAnyQueue();

        world.System(*types, *system_components_queue,
            [callback](ECS_Entity entity, Internal::AnyQueue& components, bool* stop) {
                components.InitPopBack();
                callback((Entity)entity, stop, (Types&)(*(Types*)(components.PopBack().value))...);
            });

        world.DoneWithAnyQueue(system_components_queue);
        world.DoneWithTypeQueue(types);
    }
};
}
