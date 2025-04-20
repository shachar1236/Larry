#pragma once
/* #include "ECS.h" */
#include "ECS.h"
#include "ECS/Utils.hpp"
#include "Utils/Log.h"
#include "Utils/LarryMemory.h"
#include "ECS/TypeManager.hpp"
#include "ECS/UnknownTypeVector.hpp"
#include "ECS/Entity.hpp"
#include "ECS/TypesBitmap.hpp"
#include <functional>
#include <strings.h>
#include <optional>
#include <unordered_map>
#include <queue>
#include <vector>

namespace Larry::ECS {

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

            template<typename ...Types, typename F>
            void SetComponents(int index, const F& set_callback) {
                set_callback((Types&)(*(components[type_manager->GetTypeBitmap<Types>()].GetRawByIndex(index)))...);
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

            template<typename ...Types, typename F>
            void CallFunctionWithComponentsImplamentation(
                TypeWithHidden<TypesBitmap, Types>... types,
                TypesBitmap singeltons_types,
                std::unordered_map<TypesBitmap, Scope<byte[]>>& singeltons,
                const F& callback) 
            {
                int size = entitys.size();
                for (int i = 0; i < size; i++) {
                    if (entitys[i].alive) {
                        callback((Types&)(*(singeltons_types.Intersect(types.value) ? singeltons[types.value].get() : components[types.value].GetRawByIndex(i)))...);
                    }
                }
            }

            template<typename ...Types, typename F>
            void CallFunctionWithComponents(TypesBitmap singeltons_types, std::unordered_map<TypesBitmap, Scope<byte[]>>& singeltons, const F& callback) {
                CallFunctionWithComponentsImplamentation<Types...>(
                        { type_manager->GetTypeBitmap<Types>() }...,
                        singeltons_types,
                        singeltons,
                        callback);
            }

            template<typename ...Types, typename F>
            void CallFunctionWithComponentsAdvancedImplamentation(
                TypeWithHidden<TypesBitmap, Types>... types,
                TypesBitmap singeltons_types,
                std::unordered_map<TypesBitmap, Scope<byte[]>>& singeltons,
                const F& callback) 
            {
                bool stop = false;
                BreakFunction break_func = [&](){ stop = true; };
                int size = entitys.size();
                for (int i = 0; i < size && !stop; i++) {
                    if (entitys[i].alive) {
                        Entity entity;
                        entity.alive = true;
                        entity.id = entitys[i].id;
                        entity.index = i;
                        entity.components_types = types_bitmap;
                        callback(entity, break_func, (Types&)(*(singeltons_types.Intersect(types.value) ? singeltons[types.value].get() : components[types.value].GetRawByIndex(i)))...);
                    }
                }
            }

            template<typename ...Types, typename F>
            void CallFunctionWithComponentsAdvanced(TypesBitmap singeltons_types, std::unordered_map<TypesBitmap, Scope<byte[]>>& singeltons, const F& callback) {
                CallFunctionWithComponentsAdvancedImplamentation<Types...>(
                        { type_manager->GetTypeBitmap<Types>() }...,
                        singeltons_types,
                        singeltons,
                        callback);
            }
    };

}
