#pragma once
#include "ECS/Internal/TypeManager.hpp"
#include "ECS/Internal/TypesBitmap.hpp"
#include "ECS/Internal/Entity.hpp"
#include "ECS/Internal/Queues.h"
#include "ECS/Internal/Archetype.hpp"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <optional>
#include <unordered_set>
#include <vector>
#include "ECS/ECS_C.h"
#include "Internal/Pool.hpp"
#include "Utils/Log.h"
#include "TypesBitmap.hpp"

namespace Larry::ECS::Internal {

    using TypeHashCode = ECS_TypeHashCode;

    struct EntityWithArchtype {
        Entity entity;
        Archetype* archtype;
        int index_in_archetype;
    };

    class World {
        private:
            std::vector<EntityWithArchtype> entitys;
            int dead_entites_number = 0;
            int next_dead_entity;

            TypeManager* type_manager;

            std::unordered_map<TypesBitmap, Archetype> archetypes;

            // a map where the key is a type and the value is a set of all the archetypes containing that type
            std::unordered_map<TypesBitmap, std::unordered_set<Archetype*>> type_to_archetypes;

            TypesBitmap singeltons_bitmap;
            std::unordered_map<TypesBitmap, byte*> singeltons;

            Pool<AnyQueue> any_queues;
            Pool<TypeQueue> type_queues;


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

            // returns an entity if its alive
            std::optional<const EntityWithArchtype> GetEntity(Entity entity) {
                Entity id = GetEntityIdentifier(entity);
                if (id >= entitys.size()) {
                    LA_CORE_ERROR("Entity {} id is bigger than entitys number", (int64_t)entity);
                    return std::nullopt;
                }
                EntityWithArchtype r = entitys[id];
                if (entity == r.entity) {
                    return entitys[id];
                }
                return std::nullopt;
            }
        public:
            World() {
                type_manager = new TypeManager();
            }

            ~World() {
                delete type_manager;
                for (auto&& [_, singelton] : singeltons) {
                    delete[] singelton;
                }
            }

            inline TypeManager* GetTypeManager() {
                return type_manager;
            }

            AnyQueue* InitAnyQueue() {
                return any_queues.Get();
            }

            void DoneWithAnyQueue(AnyQueue* queue) {
                queue->Clear();
                any_queues.Return(queue);
            }

            TypeQueue* InitTypeQueue() {
                return type_queues.Get();
            }

            void DoneWithTypeQueue(TypeQueue* queue) {
                queue->elements.clear();
                type_queues.Return(queue);
            }


            Entity CreateEntity() {
                if (dead_entites_number > 0) {
                    int index = next_dead_entity;
                    Entity res = (int32_t)next_dead_entity | ((int64_t)GetEntityVersion(entitys[index].entity) << 32);
                    next_dead_entity = (int32_t)entitys[next_dead_entity].entity;
                    dead_entites_number--;
                    entitys[index] = { res, nullptr };
                    return res;
                }
                Entity res = entitys.size();
                entitys.push_back({ res, nullptr, 0 });
                return res;
            }
            
            bool IsEntityAlive(Entity entity) {
                return GetEntity(entity).has_value();
            }

            // kills an entity
            void KillEntity(Entity entity) {
                std::optional<const EntityWithArchtype> fullEntityOpt = GetEntity(entity);
                if (fullEntityOpt.has_value()) {
                    const EntityWithArchtype fullEntity = fullEntityOpt.value();
                    Archetype* archetype = fullEntity.archtype;
                    if (archetype != nullptr) {
                        archetype->KillEntity(entity);
                        int32_t index = GetEntityIdentifier(entity);
                        int64_t version = GetEntityVersion(entity) + 1;
                        entitys[index].entity = (int32_t)next_dead_entity | version << 32;
                        next_dead_entity = index;
                        dead_entites_number++;
                    }
                }
            }

            // returns singeltom address
            void* CreateSingelton(ECS_TypeHashCode singelton_hash) {
                TypesBitmap type = type_manager->GetTypeBitmap(singelton_hash);
                if (singeltons.find(type) == singeltons.end()) {
                    singeltons[type] = new byte[type_manager->GetTypeSize(singelton_hash)];
                }
                singeltons_bitmap = singeltons_bitmap | type;
                return singeltons[type];
            }

            std::optional<void*> GetSingelton(ECS_TypeHashCode hash) {
                TypesBitmap type = type_manager->GetTypeBitmap(hash);
                auto res = singeltons.find(type) ;
                if (res == singeltons.end()) {
                    return std::nullopt;
                }
                return res->second;
            }

            // Inserts component to entity
            // return - if completed successfully
            bool InsertComponents(Entity entity, const TypeQueue& types, AnyQueue& resultQueue) {
                std::optional<const EntityWithArchtype> fullEntityOpt = GetEntity(entity);
                if (fullEntityOpt.has_value()) {
                    const EntityWithArchtype fullEntity = fullEntityOpt.value();
                    TypesBitmap entity_components = fullEntity.archtype != nullptr ? fullEntity.archtype->GetTypesBitmap() : TypesBitmap();
                    TypesBitmap new_bitmap = entity_components | type_manager->QueueTypes(types);

                    bool type_in_singeltons = !(new_bitmap & singeltons_bitmap).IsNull();
                    if (!type_in_singeltons) {
                        Archetype* new_archetype = GetArchetype(new_bitmap);
                        int entity_index = GetEntityIdentifier(entity);

                        if (!entity_components.IsNull()) {
                            Archetype* old_archetype = fullEntity.archtype;
                            int index = new_archetype->PopEntityFromOtherArchetype(entity, old_archetype, fullEntity.index_in_archetype);
                            new_archetype->SetComponents(index, types, resultQueue);
                            
                            entitys[entity_index].archtype = new_archetype;
                            entitys[entity_index].index_in_archetype = index;
                        } else {
                            int index = new_archetype->AllocateNew(entity);
                            new_archetype->SetComponents(index, types, resultQueue);

                            entitys[entity_index].archtype = new_archetype;
                            entitys[entity_index].index_in_archetype = index;
                        }

                        // update entity to match the new data
                        return true;
                    }
                }
                return false;
            }

            // returns if completed succesfully
            bool SetComponents(Entity entity, const TypeQueue& types, AnyQueue& resultQueue) {
                std::optional<const EntityWithArchtype> fullEntityOpt = GetEntity(entity);
                if (fullEntityOpt.has_value()) {
                    const EntityWithArchtype fullEntity = fullEntityOpt.value();
                    Archetype* archetype = fullEntity.archtype;

                    TypesBitmap types_bitmap = type_manager->QueueTypes(types);
                    bool has_types = (types_bitmap & archetype->GetTypesBitmap()) == types_bitmap;
                    if (has_types) {
                        archetype->SetComponents(fullEntity.index_in_archetype, types, resultQueue);
                        return true;
                    }
                }
                return false;
            }

            std::optional<ECS_Any> GetComponent(Entity entity, ECS_TypeHashCode type_hash) {
                std::optional<const EntityWithArchtype> fullEntityOpt = GetEntity(entity);
                if (fullEntityOpt.has_value()) {
                    const EntityWithArchtype fullEntity = fullEntityOpt.value();
                    Archetype* archetype = fullEntity.archtype;
                    return archetype->GetComponent(fullEntity.index_in_archetype, type_hash);
                }
                return std::nullopt;
            }

            void DeleteComponent(Entity entity, ECS_TypeHashCode type_hash) {
                std::optional<const EntityWithArchtype> fullEntityOpt = GetEntity(entity);
                if (fullEntityOpt.has_value()) {
                    const EntityWithArchtype fullEntity = fullEntityOpt.value();
                    if (fullEntity.archtype != nullptr) {
                        TypesBitmap entityTypes = fullEntity.archtype->GetTypesBitmap();
                        TypesBitmap new_bitmap = entityTypes & (~type_manager->GetTypeBitmap(type_hash));
                        Archetype* new_archetype = GetArchetype(new_bitmap);

                        if (!entityTypes.IsNull()) {
                            Archetype* old_archetype = fullEntity.archtype;
                            int index = new_archetype->PopEntityFromOtherArchetype(entity, old_archetype, fullEntity.index_in_archetype);
                            int entity_index = GetEntityIdentifier(entity);

                            entitys[entity_index].index_in_archetype = index;
                            entitys[entity_index].archtype = new_archetype;
                        }
                    }
                }
            }

        public:
            template<typename F>
                void System(const TypeQueue& type_queue, AnyQueue& system_components_queue, const F& callback) {
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
