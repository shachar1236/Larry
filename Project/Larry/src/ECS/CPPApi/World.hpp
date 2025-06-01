#pragma once
#include "ECS/CPPApi/ECS.h"
#include "ECS/ECS_C.h"
#include "ECS/Internal/Queues.h"
#include "ECS/Internal/TypeManager.hpp"
#include "ECS/Internal/World.hpp"
#include "Utils/Log.h"
#include <optional>
#include <type_traits> // For std::is_default_constructible, std::enable_if

#define TypeHash(T) typeid(T).hash_code()
#define RegisterTypes(Ts) (ECS_RegisterType(&world, TypeHash(Ts), sizeof(Ts), DESTRUCTOR_LAMBDA(Ts)), ...);
#define ForEachType(x) (x, ...)
// #define CallFWithAnyQueue(queue) F((Types&)(*(Types*)(queue->Pop().value))...)

// --- Function for types with a default constructor ---
template <typename T,
          typename std::enable_if<std::is_default_constructible<T>::value, int>::type = 0>
void CallDefaultConstractorIfAvailable(T* obj) {
    // Call the default constructor using placement new.
    // Be cautious if 'obj' was already a valid object and manages resources,
    // as the old object's destructor won't be called automatically.
    new (obj) T();
    // You can add more specific logic here for default-constructible types
}

// --- Function for types WITHOUT a default constructor ---
template <typename T,
          typename std::enable_if<!std::is_default_constructible<T>::value, int>::type = 0>
void CallDefaultConstractorIfAvailable(T* obj) {
    LA_CORE_WARN("ECS: component dosent have default constractor, cant call it.");
}

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

                Internal::AnyQueue* resultQueue = world.InitAnyQueue();
                Internal::TypeQueue* types = world.InitTypeQueue();
                // ForEachType(LA_CORE_DEBUG("Type hash: {}", TypeHash(Types)));
                ForEachType(types->push_back(TypeHash(Types)));
                bool success = world.InsertComponents(entity, *types, *resultQueue);

                if (success) {
                    resultQueue->InitPopBack();
                    ForEachType(CallDefaultConstractorIfAvailable((Types*)(resultQueue->PopBack().value)));
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

            template <typename T> 
            T* GetSingelton()
            {
                ECS_RegisterType(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

                std::optional<void*> res = world.GetSingelton(TypeHash(T));
                void* res_val;
                if (!res.has_value()) {
                    res_val = world.CreateSingelton(TypeHash(T));
                    CallDefaultConstractorIfAvailable((T*)res_val);
                } else {
                    res_val = res.value();
                }
                return (T*)res_val;
            }

            template <typename T, typename F> 
            void SetSingelton(const F& set_callback)
            {
                ECS_RegisterType(&world, TypeHash(T), sizeof(T), DESTRUCTOR_LAMBDA(T));

                std::optional<void*> res = world.GetSingelton(TypeHash(T));
                void* res_val;
                if (!res.has_value()) {
                    res_val = world.CreateSingelton(TypeHash(T));
                    CallDefaultConstractorIfAvailable((T*)res_val);
                } else {
                    res_val = res.value();
                }
                set_callback(*(T*)res_val);
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
