#pragma once
/* #include "ECS.h" */
#include "ECS/Internal/Utils.hpp"
#include "ECS_C.h"
#include "Internal/Queues.h"
#include "TypesBitmap.hpp"
#include "Utils/Log.h"
#include "Utils/LarryMemory.h"
#include "ECS/Internal/TypeManager.hpp"
#include "ECS/Internal/UnknownTypeVector.hpp"
#include "ECS/Internal/Entity.hpp"
#include "ECS/Internal/TypesBitmap.hpp"
#include <functional>
#include <strings.h>
#include <optional>
#include <unordered_map>
#include <queue>
#include <vector>

namespace Larry::ECS::Internal {

    class Archetype {
        private:
            TypeManager* type_manager;

            TypesBitmap types_bitmap;

            std::vector<EncodedEntity> entitys;
            std::unordered_map<TypesBitmap, UnknownTypeVector> components;

            std::queue<int> dead_entites;
        public:
            Archetype() {

            }

            Archetype(TypesBitmap types_bitmap_, TypeManager* type_manager_) :
                types_bitmap(types_bitmap_),
                type_manager(type_manager_)
        {
            types_bitmap.ForEachType([&](TypesBitmap curr){
                    components[curr] = UnknownTypeVector(type_manager->GetTypeSize(curr));
                    });
        }

            bool operator==(const Archetype& other) {
                return this->types_bitmap == other.types_bitmap;
            }

            TypesBitmap GetTypesBitmap() {
                return types_bitmap;
            }

            int GetSize() {
                return entitys.size();
            }

            bool IsAlive(const Entity& entity) {
                EncodedEntity e = entitys[entity.index];
                return e.id == entity.id && e.alive && types_bitmap == entity.components_types;
            }

            void KillEntity(Entity& entity, bool destruct=true) {
                entitys[entity.index].alive = false;
                entity.alive = false;
                /* for (auto& [_, value] : components) { */
                /* value.Clear(entity.index); */
                /* } */
                dead_entites.push(entity.index);
                if (destruct) {
                    DestructComponents(entity.index, types_bitmap);
                }
            }

            void DestructComponents(int index, TypesBitmap types) {
                types.ForEachType([&](TypesBitmap curr){
                        byte* object = components[curr].GetRawByIndex(index);
                        type_manager->DestructType(curr, object);
                        });
            }

            std::optional<Entity> GetEntityById(UID id) {
                for (int i = 0; i < entitys.size(); i++) {
                    if (entitys[i].alive && entitys[i].id == id) {
                        Entity new_entity(id);
                        new_entity.index = i;
                        new_entity.components_types = types_bitmap;
                        return new_entity;
                    }
                }
                return std::nullopt;
            }

            std::optional<ECS_Any> GetComponent(const Entity& entity, ECS_TypeHashCode type_hash) {
                TypesBitmap type = type_manager->GetTypeBitmap(type_hash);
                if ((types_bitmap & type) == type) {
                    auto component = components.find(type);
                    if (component != components.end()) {
                        return ECS_Any{component->second.GetRawByIndex(entity.index), type_hash};
                    }
                } 
                return std::nullopt;
            }

            // allocate space for new entity and returns the index
            // dosent change entity.index
            int AllocateNew(const Entity& entity) {
                // TODO: checl dead_entites queue before allocationg new data
                if  (!dead_entites.empty()) {
                    int index = dead_entites.front();
                    dead_entites.pop();
                    entitys[index] = entity.ToEncodedEntity();
                    return index;
                }

                int index = entitys.size();
                entitys.push_back(entity.ToEncodedEntity());
                for (auto& [_, value] : components) {
                    value.AllocateData();
                }
                return index;
            }

            void SetComponents(int index, const AnyQueue& values) {
                for (auto& value : values) {
                    memcpy(components[type_manager->GetTypeBitmap(value.type)].GetRawByIndex(index), value.value, type_manager->GetTypeSize(value.type));
                }
            }

            // returns index of the new entity
            // dosent change entity.index
            int PopEntityFromOtherArchetype(const Entity& entity, Archetype* other) {
                int index = AllocateNew(entity);
                int old_index = entity.index;
                TypesBitmap intesecting = types_bitmap & other->types_bitmap;
                intesecting.ForEachType([&](TypesBitmap type){
                        other->components[type].Copy(entity.index, components[type].GetRawByIndex(index));
                        });
                Entity copy_entity = Entity(entity);
                other->KillEntity(copy_entity, false);

                TypesBitmap left_on_other = (~intesecting) & other->types_bitmap;
                if (!left_on_other.IsNull()) {
                    LA_CORE_DEBUG("There are components left on the other archetype!");
                    other->DestructComponents(entity.index, left_on_other);
                }

                return index;
            }

            template<typename F>
                void CallFunctionWithComponents(
                        const TypeQueue& type_queue,
                        AnyQueue& system_components_queue,
                        TypesBitmap singeltons_types,
                        std::unordered_map<TypesBitmap, Larry::Scope<byte[]>>& singeltons,
                        const F& callback) 
                {
                    bool stop = false;
                    int size = entitys.size();
                    for (int i = 0; i < size && !stop; i++) {
                        if (entitys[i].alive) {
                            Entity entity;
                            entity.alive = true;
                            entity.id = entitys[i].id;
                            entity.index = i;
                            entity.components_types = types_bitmap;
                            system_components_queue.clear();
                            for (auto& component_type : type_queue) {
                                TypesBitmap t = type_manager->GetTypeBitmap(component_type);
                                if (singeltons_types.Intersect(t)) {
                                    system_components_queue.push_back(ECS_Any{singeltons[t].get(), component_type});
                                } else {
                                    system_components_queue.push_back(ECS_Any{components[t].GetRawByIndex(i), component_type});
                                }
                            }
                            callback(entity, system_components_queue, &stop);
                        }
                    }
                }

    };

}
