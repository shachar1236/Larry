#pragma once
#include "LarryMemory.h"
#include "Log.h"
#include "TypeManger.hpp"
#include "UnknownTypeVector.hpp"
#include "ECS/Entity.hpp"
#include "TypesBitmap.hpp"
#include <strings.h>
#include <unordered_map>
#include <vector>

namespace Larry::ECS {

    class Archetype {
        private:
            TypeManager* type_manager;
            std::vector<EncodedEntity> entitys;
            std::unordered_map<TypesBitmap, UnknownTypeVector> components;
            TypesBitmap types_bitmap;

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

            int GetSize() {
                return entitys.size();
            }

            std::optional<Entity> GetEntityById(UID id) {
                for (int i = 0; i < entitys.size(); i++) {
                    if (entitys[i].id == id) {
                        Entity new_entity(id);
                        new_entity.index = i;
                        new_entity.components_types = types_bitmap;
                        return new_entity;
                    }
                }
                return std::nullopt;
            }

            template<typename T>
            std::optional<const T*> GetComponent(const Entity& entity) {
                TypesBitmap type = type_manager->GetTypeBitmap<T>();
                if ((types_bitmap & type) == type) {
                    auto component = components.find(type);
                    if (component != components.end()) {
                        return component->second.GetByIndex<T>(entity.index);
                    }
                } 
                return std::nullopt;
            }

            // allocate space for new entity and returns the index
            // dosent change entity.index
            int AllocateNew(const Entity& entity) {
                int index = entitys.size();
                entitys.push_back(entity.ToEncodedEntity());
                for (auto& [_, value] : components) {
                    value.AllocateData();
                }
                return index;
            }

            template<typename ...Types, typename F>
            void SetComponents(int index, const F& set_callback) {
                set_callback((Types&)(*(components[type_manager->GetTypeBitmap<Types>()].GetRawByIndex(index)))...);
            }

            // returns index of the new entity
            // dosent change entity.index
            int PopEntityFromOtherArchetype(const Entity& entity, Archetype* other) {
                int index = AllocateNew(entity);
                int old_index = entity.index;
                entity.components_types.ForEachType([&](TypesBitmap type){
                    other->components[type].Pop(entity.index, components[type].GetRawByIndex(index));
                });
                other->entitys.erase(other->entitys.begin() + old_index, other->entitys.begin() + old_index + 1);
                // TODO: make it mark the entity as erased so I want have to change all of the entites in that archtype indexes

                return index;
            }

            template<typename ...Types, typename F>
            void CallFunctionWithComponents(const F& callback) {
                int size = entitys.size();
                for (int i = 0; i < size; i++) {
                    callback((Types&)(*components[type_manager->GetTypeBitmap<Types>()].GetRawByIndex(i))...);
                }
            }

    };

}
