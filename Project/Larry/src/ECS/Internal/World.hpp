#pragma once
#include "ECS/Internal/TypeManager.hpp"
#include "ECS/Internal/TypesBitmap.hpp"
#include "ECS/Internal/Entity.hpp"
#include "ECS/Internal/Queues.h"
#include "ECS/Internal/Archetype.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <optional>
#include <unordered_set>
#include <vector>
#include "ECS/Internal/Utils.hpp"
#include "ECS_C.h"
#include "Internal/Pool.hpp"

namespace Larry::ECS::Internal {

    using TypeHashCode = ECS_TypeHashCode;

    class World {
        private:
            UID family_uid = 0;

            UID FamilyGenerate() {
                return family_uid++;
            }

            TypeManager* type_manager;

            std::unordered_map<TypesBitmap, Archetype> archetypes;

            // a map where the key is a type and the value is a set of all the archetypes containing that type
            std::unordered_map<TypesBitmap, std::unordered_set<Archetype*>> type_to_archetypes;

            TypesBitmap singeltons_bitmap;
            std::unordered_map<TypesBitmap, Larry::Scope<byte[]>> singeltons;

            Pool<AnyQueue> any_queues;
            Pool<TypeQueue> type_queues;

            struct { TypesBitmap types; } system_state;

            Archetype* GetArchetype(TypesBitmap types) {
                auto res = archetypes.find(types);
                if (res != archetypes.end()) {
                    return &(res->second);
                }

                archetypes[types] = Archetype(types, type_manager);
                res = archetypes.find(types);
                Archetype* ptr = &(res->second);
                types.ForEachType([this, ptr](TypesBitmap curr){
                        type_to_archetypes[curr].insert(ptr);
                        });

                return ptr;
            }

        public:
            World() {
                type_manager = new TypeManager();
            }

            ~World() {
                delete type_manager;
            }

            AnyQueue* InitAnyQueue() {
                return any_queues.Get();
            }

            void DoneWithAnyQueue(AnyQueue* queue) {
                any_queues.Return(queue);
            }

            TypeQueue* InitTypeQueue() {
                return type_queues.Get();
            }

            void DoneWithTypeQueue(TypeQueue* queue) {
                type_queues.Return(queue);
            }


            Entity CreateEntity() {
                UID id = FamilyGenerate();
                Entity entity = Entity(id);
                return entity;
            }
            
            // should return an entity with the given id
            // WARNING: an expensive function avoid if possible
            std::optional<Entity> GetEntity(UID entity_id) {
                for (auto& archetype : archetypes) {
                    auto res = archetype.second.GetEntityById(entity_id);
                    if (res.has_value()) {
                        return res;
                    }
                }
                return std::nullopt;
            }

            // kills an entity
            void KillEntity(Entity& entity) {
                Archetype* archetype = GetArchetype(entity.components_types);
                if (archetype->IsAlive(entity)) {
                    archetype->KillEntity(entity);
                }
            }

            // returns singeltom address
            void* CreateSingelton(ECS_Any singelton) {
                TypesBitmap type = type_manager->GetTypeBitmap(singelton.type);
                if (singeltons.find(type) == singeltons.end()) {
                    singeltons[type] = Larry::CreateScope<byte[]>(type_manager->GetTypeSize(singelton.type));
                    singeltons_bitmap = singeltons_bitmap | type;
                    return singeltons[type].get();
                }
            }

            void* GetSingelton(ECS_TypeHashCode hash) {
                TypesBitmap type = type_manager->GetTypeBitmap(hash);
                auto res = singeltons.find(type) ;
                assert(res != singeltons.end());
                return res->second.get();
            }

            // Inserts component to entity
            // return - if completed successfully
            bool InsertComponent(Entity& entity, const AnyQueue& components) {
                TypesBitmap new_bitmap = entity.components_types;
                for (auto& component : components) {
                    new_bitmap = new_bitmap | type_manager->GetTypeBitmap(component.type);
                }

                bool type_in_singeltons = !(new_bitmap & singeltons_bitmap).IsNull();
                if (entity.alive && !type_in_singeltons) {
                    Archetype* new_archetype = GetArchetype(new_bitmap);

                    if (!entity.components_types.IsNull()) {
                        Archetype* old_archetype = GetArchetype(entity.components_types);
                        if (old_archetype->IsAlive(entity)) {
                            int index = new_archetype->PopEntityFromOtherArchetype(entity, old_archetype);
                            new_archetype->SetComponents(index, components);

                            entity.index = index;
                        } else {
                            entity.alive = false;
                            return false;
                        }
                    } else {
                        int index = new_archetype->AllocateNew(entity);
                        new_archetype->SetComponents(index, components);

                        entity.index = index;
                    }

                    // update entity to match the new data
                    entity.components_types = new_bitmap;
                    return true;
                }
                return false;
            }

            void SetComponents(const Entity& entity, const AnyQueue& components) {
                if (entity.alive) {
                    Archetype* archetype = GetArchetype(entity.components_types);
                    if (archetype->IsAlive(entity)) {
                        archetype->SetComponents(entity.index, components);
                    }
                }
            }

            std::optional<ECS_Any> GetComponent(const Entity& entity, ECS_TypeHashCode type_hash) {
                if (entity.alive) {
                    Archetype* archetype = GetArchetype(entity.components_types);
                    if (archetype->IsAlive(entity)) {
                        return archetype->GetComponent(entity, type_hash);
                    }
                }
                return std::nullopt;
            }

            void DeleteComponent(Entity& entity, ECS_TypeHashCode type_hash) {
                if (entity.alive) {
                    TypesBitmap new_bitmap = entity.components_types & (~type_manager->GetTypeBitmap(type_hash));
                    Archetype* new_archetype = GetArchetype(new_bitmap);

                    if (!entity.components_types.IsNull()) {
                        Archetype* old_archetype = GetArchetype(entity.components_types);
                        if (old_archetype->IsAlive(entity)) {
                            int index = new_archetype->PopEntityFromOtherArchetype(entity, old_archetype);

                            entity.index = index;
                            entity.components_types = new_bitmap;
                        }

                    }
                }
            }

        private:
            template<typename F>
                void LoopArchetypes(const TypesBitmap& types, const F& callback) {
                }

        public:
            template<typename F>
                void System(TypeQueue type_queue, AnyQueue& system_components_queue, const F& callback) {
                    // TODO: optimize this
                    TypesBitmap types = (~singeltons_bitmap) & (type_manager->QueueTypes(type_queue));
                    int shortest = -1;
                    std::unordered_set<Archetype*> my_archetypes;
                    types.ForEachType([&](TypesBitmap curr){
                        std::unordered_set<Archetype*> c = type_to_archetypes[curr];
                        if (c.size() < my_archetypes.size() || shortest == -1) {
                            my_archetypes = c;
                        }
                    });

                    for (auto& archetype : my_archetypes) {
                        bool has_types = (archetype->GetTypesBitmap() & types) == types;
                        if (has_types) {
                            archetype->CallFunctionWithComponents(type_queue, system_components_queue, singeltons_bitmap, singeltons, callback);
                        }
                    }
                }
    };
}
