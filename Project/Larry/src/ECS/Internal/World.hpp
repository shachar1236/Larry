#pragma once
#include "ECS/Internal/ArchetypesGraph.hpp"
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
        int archtype_index;
        int index_in_archetype;
    };

    class World {
        private:
            std::vector<EntityWithArchtype> entitys;
            int dead_entites_number = 0;
            int next_dead_entity;

            TypeManager type_manager;

            ArchetypesGraph archetypes;

            TypesBitmap singeltons_bitmap;
            std::unordered_map<TypesBitmap, byte*> singeltons;

            Pool<AnyQueue> any_queues;
            Pool<TypeQueue> type_queues;

            // returns archetype index
            int GetArchetype(const TypesBitmap& types) {
                int ptr = archetypes.GetArchetype(types);
                if (ptr != -1) {
                    return ptr;
                }

                LA_CORE_INFO("ECS: Creating archtype for {}", std::hash<TypesBitmap>()(types));
                ptr = archetypes.CreateArchetype(types, &type_manager);
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
            }

            ~World() {
                for (auto&& [_, singelton] : singeltons) {
                    delete[] singelton;
                }
            }

            inline TypeManager* GetTypeManager() {
                return &type_manager;
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
                    entitys[index] = { res, -1, -1 };
                    return res;
                }
                Entity res = entitys.size();
                entitys.push_back({ res, -1, 0 });
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
                    if (fullEntity.archtype_index != -1) {
                        Archetype& archetype = archetypes.GetArchetypeByIndex(fullEntity.archtype_index);
                        archetype.KillEntity(entity);
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
                TypesBitmap type = type_manager.GetTypeBitmap(singelton_hash);
                if (singeltons.find(type) == singeltons.end()) {
                    singeltons[type] = new byte[type_manager.GetTypeSize(singelton_hash)];
                }
                singeltons_bitmap = singeltons_bitmap | type;
                return singeltons[type];
            }

            std::optional<void*> GetSingelton(ECS_TypeHashCode hash) {
                TypesBitmap type = type_manager.GetTypeBitmap(hash);
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
                    TypesBitmap entity_components = fullEntity.archtype_index != -1 ? archetypes.GetArchetypeByIndex(fullEntity.archtype_index).GetTypesBitmap() : TypesBitmap();
                    TypesBitmap new_bitmap = entity_components | type_manager.QueueTypes(types);

                    bool type_in_singeltons = !(new_bitmap & singeltons_bitmap).IsNull();
                    if (!type_in_singeltons) {
                        int new_archetype_index = GetArchetype(new_bitmap);
                        Archetype& new_archetype = archetypes.GetArchetypeByIndex(new_archetype_index);
                        int entity_index = GetEntityIdentifier(entity);

                        if (!entity_components.IsNull()) {
                            Archetype& old_archetype = archetypes.GetArchetypeByIndex(fullEntity.archtype_index);
                            int index = new_archetype.PopEntityFromOtherArchetype(entity, old_archetype, fullEntity.index_in_archetype);
                            new_archetype.SetComponents(index, types, resultQueue);
                            
                            entitys[entity_index].archtype_index = new_archetype_index;
                            entitys[entity_index].index_in_archetype = index;
                        } else {
                            int index = new_archetype.AllocateNew(entity);
                            new_archetype.SetComponents(index, types, resultQueue);

                            entitys[entity_index].archtype_index = new_archetype_index;
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
                    Archetype& archetype = archetypes.GetArchetypeByIndex(fullEntity.archtype_index);

                    TypesBitmap types_bitmap = type_manager.QueueTypes(types);
                    bool has_types = (types_bitmap & archetype.GetTypesBitmap()) == types_bitmap;
                    if (has_types) {
                        archetype.SetComponents(fullEntity.index_in_archetype, types, resultQueue);
                        return true;
                    }
                }
                return false;
            }

            std::optional<ECS_Any> GetComponent(Entity entity, ECS_TypeHashCode type_hash) {
                std::optional<const EntityWithArchtype> fullEntityOpt = GetEntity(entity);
                if (fullEntityOpt.has_value()) {
                    const EntityWithArchtype fullEntity = fullEntityOpt.value();
                    Archetype& archetype = archetypes.GetArchetypeByIndex(fullEntity.archtype_index);
                    return archetype.GetComponent(fullEntity.index_in_archetype, type_hash);
                }
                return std::nullopt;
            }

            void DeleteComponent(Entity entity, ECS_TypeHashCode type_hash) {
                std::optional<const EntityWithArchtype> fullEntityOpt = GetEntity(entity);
                if (fullEntityOpt.has_value()) {
                    const EntityWithArchtype fullEntity = fullEntityOpt.value();
                    if (fullEntity.archtype_index != -1) {
                        Archetype& archetype = archetypes.GetArchetypeByIndex(fullEntity.archtype_index);
                        TypesBitmap entityTypes = archetype.GetTypesBitmap();

                        TypesBitmap new_bitmap = entityTypes & (~type_manager.GetTypeBitmap(type_hash));
                        int new_archetype_index = GetArchetype(new_bitmap);
                        Archetype& new_archetype = archetypes.GetArchetypeByIndex(new_archetype_index);

                        if (!entityTypes.IsNull()) {
                            int index = new_archetype.PopEntityFromOtherArchetype(entity, archetype, fullEntity.index_in_archetype);
                            int entity_index = GetEntityIdentifier(entity);

                            entitys[entity_index].index_in_archetype = index;
                            entitys[entity_index].archtype_index = new_archetype_index;
                        }
                    }
                }
            }

            template<typename F>
            void System(const TypeQueue& type_queue, AnyQueue& system_components_queue, const F& f_callback) {
                TypesBitmap types = (~singeltons_bitmap) & (type_manager.QueueTypes(type_queue));
                archetypes.CallWithIntersectingArchetypes(types, [this, &types, &f_callback, &type_queue, &system_components_queue](Archetype* archetype, bool* stop){
                    archetype->CallFunctionWithComponents(type_queue, system_components_queue, singeltons_bitmap, singeltons, f_callback, stop);
                    LA_CORE_INFO("hiiww");
                });
            }
    };
}
