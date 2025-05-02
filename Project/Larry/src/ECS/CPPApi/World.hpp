#pragma once
#include "ECS/ECS_C.h"
#include "ECS/CPPApi/ECS.h"
#include "ECS/Internal/World.hpp"
#include "ECS/Internal/TypeManager.hpp"

#define TypeHash(T) typeid(T).hash_code()
#define RegisterTypes(Ts) (ECS_RegisterTypeIfDosentExists(&world, TypeHash(Ts), sizeof(Ts), DESTRUCTOR_LAMBDA(Ts)), ...);
#define ForEachType(x) (x, ...)
#define CallFWithAnyQueue(queue) F(((*(Types*)(queue->Pop().value)))...)

namespace Larry::ECS {
    class World {
    private:
        Internal::World world;

    public:
        World() {
            world = Internal::World();
        }

        ~World() {

        }

        Entity CreateEntity() {
            return ECS_CreateEntity(&world);
        }

        bool IsEntityAlive(Entity entity) {
            return world.IsEntityAlive(entity);
        }

        void KillEntity(Entity entity) {
            world.KillEntity(entity);
        }

        template<typename ...Types, typename F>
        bool InsertComponent(Entity entity, const F& set_callback) {
            RegisterTypes(Types);

            Internal::AnyQueue* resultQueue = world.InitAnyQueue();
            Internal::TypeQueue* types = world.InitTypeQueue();
            ForEachType(types->push_back(TypeHash(Types)));
            world.InsertComponents(entity, *types, *resultQueue);
            
            CallFWithAnyQueue(resultQueue);

            world.DoneWithAnyQueue(resultQueue);
            world.DoneWithTypeQueue(types);
        }

        template<typename T>
        const T& GetComponent(Entity entity) {
            ECS_RegisterTypeIfDosentExists(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

            return (T&)(*(T*)world.GetComponent(entity, TypeHash(T)).value_or(Any{ NULL, 0 }).value);
        }

        template<typename ...Types, typename F>
        bool SetComponents(Entity entity, const F& set_callback) {
            RegisterTypes(Types);

            Internal::AnyQueue* resultQueue = world.InitAnyQueue();
            Internal::TypeQueue* types = world.InitTypeQueue();
            ForEachType(types->push_back(TypeHash(Types)));
            world.SetComponents(entity, *types, *resultQueue);

            CallFWithAnyQueue(resultQueue);

            world.DoneWithAnyQueue(resultQueue);
            world.DoneWithTypeQueue(types);
        }
        
        template<typename T>
        void DeleteComponent(Entity entity) {
            ECS_RegisterTypeIfDosentExists(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

            world.DeleteComponent(entity, TypeHash(T));
        }

        template<typename T, typename F>
        void CreateSingelton(const F& set_callback) {
            ECS_RegisterTypeIfDosentExists(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

            T* res = (T*)world.CreateSingelton(TypeHash(T));
            set_callback(*res);
        }

        template<typename T>
        std::optional<T*> GetSingelton() {
            ECS_RegisterTypeIfDosentExists(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

            std::optional<T*> res = world.GetSingelton(TypeHash(T));
            return res;
        }

        template<typename T, typename F>
        void SetSingelton(const F& set_callback) {
            ECS_RegisterTypeIfDosentExists(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

            std::optional<T*> res = world.GetSingelton(TypeHash(T));
            if (res.has_value()) {
                set_callback(*res.value());
            }
        }

        template<typename ...Types, typename F>
        void System(const F& callback) {
            RegisterTypes(Types);

            Internal::TypeQueue* types = world.InitTypeQueue();
            ForEachType(types->push_back(TypeHash(Types)));

            Internal::AnyQueue* system_components_queue = world.InitAnyQueue();

            world.System(types, *system_components_queue, [](ECS_Entity entity, AnyQueue& components, bool* stop){
                F(((*(Types*)(components.Pop().value)))...);
            });

            world.DoneWithAnyQueue(system_components_queue);
            world.DoneWithTypeQueue(types);
        }

        template<typename ...Types, typename F>
        void AdvancedSystem(const F& callback) {
            RegisterTypes(Types);

            Internal::TypeQueue* types = world.InitTypeQueue();
            ForEachType(types->push_back(TypeHash(Types)));

            Internal::AnyQueue* system_components_queue = world.InitAnyQueue();

            world.System(types, *system_components_queue, [](ECS_Entity entity, AnyQueue& components, bool* stop){
                F(entity, stop, ((*(Types*)(components.Pop().value)))...);
            });

            world.DoneWithAnyQueue(system_components_queue);
            world.DoneWithTypeQueue(types);
        }
    };
}
