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

            struct EntityWithState {
                Entity entity;
                bool alive = true;
            };
            std::vector<EntityWithState> entitys;
            std::unordered_map<TypesBitmap, UnknownTypeVector> components;

            int dead_entites_number = 0;
            int next_dead_entity;
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

            void KillEntity(int entity_index, bool destruct=true) {
                if (destruct) {
                    DestructComponents(entity_index, types_bitmap);
                }

                entitys[entity_index].entity = next_dead_entity;
                entitys[entity_index].alive = false;
                next_dead_entity = entity_index;
                dead_entites_number++;
            }

            void DestructComponents(int index, TypesBitmap types) {
                types.ForEachType([&](TypesBitmap curr){
                    byte* object = components[curr].GetRawByIndex(index);
                    type_manager->DestructType(curr, object);
                });
            }

            std::optional<ECS_Any> GetComponent(int entity_index, ECS_TypeHashCode type_hash) {
                TypesBitmap type = type_manager->GetTypeBitmap(type_hash);
                if ((types_bitmap & type) == type) {
                    auto component = components.find(type);
                    if (component != components.end()) {
                        return ECS_Any{component->second.GetRawByIndex(entity_index), type_hash};
                    }
                } 
                return std::nullopt;
            }

            // allocate space for new entity and returns the index
            // dosent change entity.index
            int AllocateNew(Entity entity) {
                if  (dead_entites_number > 0) {
                    int index = next_dead_entity;

                    if (dead_entites_number > 1) {
                        next_dead_entity = entitys[next_dead_entity].entity;
                    }
                    dead_entites_number--;

                    entitys[index].entity = entity;
                    entitys[index].alive = true;
                    return index;
                }

                int index = entitys.size();
                entitys.push_back({ entity, true });
                for (auto& [_, value] : components) {
                    value.AllocateData();
                }
                return index;
            }

            void SetComponents(int index, const AnyQueue& values) {
                for (auto& value : values.elements) {
                    memcpy(components[type_manager->GetTypeBitmap(value.type)].GetRawByIndex(index), value.value, type_manager->GetTypeSize(value.type));
                }
            }

            // returns index of the new entity
            // dosent change entity.index
            int PopEntityFromOtherArchetype(Entity entity, Archetype* other, int entity_index_in_other_archtype) {
                int index = AllocateNew(entity);
                int old_index = entity_index_in_other_archtype;
                TypesBitmap intesecting = types_bitmap & other->types_bitmap;

                intesecting.ForEachType([&](TypesBitmap type){
                    other->components[type].Copy(old_index, components[type].GetRawByIndex(index));
                });
                other->KillEntity(entity, false);

                TypesBitmap left_on_other = (~intesecting) & other->types_bitmap;
                if (!left_on_other.IsNull()) {
                    LA_CORE_DEBUG("There are components left on the other archetype!");
                    other->DestructComponents(old_index, left_on_other);
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
                            system_components_queue.Clear();
                            for (auto& component_type : type_queue) {
                                TypesBitmap t = type_manager->GetTypeBitmap(component_type);
                                if (singeltons_types.Intersect(t)) {
                                    system_components_queue.elements.push_back(ECS_Any{singeltons[t].get(), component_type});
                                } else {
                                    system_components_queue.elements.push_back(ECS_Any{components[t].GetRawByIndex(i), component_type});
                                }
                            }
                            callback(entitys[i].entity, system_components_queue, &stop);
                        }
                    }
                }

    };

}
