#pragma once
#include "ECS/TypeManager.hpp"
#include "ECS/TypesBitmap.hpp"
#include "Entity.hpp"
#include "Archetype.hpp"
#include "LarryMemory.h"
#include "Log.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <functional>
#include <optional>
#include <typeinfo>
#include <unordered_set>
#include <vector>

namespace Larry::ECS {

    using TypeHashCode = size_t;

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
            archetype->KillEntity(entity);
        }

        Entity CreateEntity() {
            UID id = FamilyGenerate();
            Entity entity = Entity(id);
            return entity;
        }

        
        template<typename ...Types, typename F>
        void InsertComponent(Entity& entity, const F& set_callcack) {
            if (entity.alive) {
                TypesBitmap new_bitmap = entity.components_types | (type_manager->GetTypeBitmap<Types>() | ...);
                Archetype* new_archetype = GetArchetype(new_bitmap);

                if (!entity.components_types.IsNull()) {
                    Archetype* old_archetype = GetArchetype(entity.components_types);

                    int index = new_archetype->PopEntityFromOtherArchetype(entity, old_archetype);
                    new_archetype->SetComponents<Types...>(index, set_callcack);

                    entity.index = index;
                } else {
                    int index = new_archetype->AllocateNew(entity);
                    new_archetype->SetComponents<Types...>(index, set_callcack);

                    entity.index = index;
                }

                // update entity to match the new data
                entity.components_types = new_bitmap;
            }
        }

        template<typename ...Types, typename F>
        void SetComponents(const Entity& entity, const F& set_callcack) {
            if (entity.alive) {
                Archetype* archetype = GetArchetype(entity.components_types);
                archetype->SetComponents<Types...>(entity.index, set_callcack);
            }
        }

        template<typename T>
        std::optional<const T*> GetComponent(const Entity& entity) {
            if (entity.alive) {
                Archetype* archetype = GetArchetype(entity.components_types);
                return archetype->GetComponent<T>(entity);
            }
            return std::nullopt;
        }

        template<typename T>
        void DeleteComponent(Entity& entity) {
            if (entity.alive) {
                TypesBitmap new_bitmap = entity.components_types & (~type_manager->GetTypeBitmap<T>());
                Archetype* new_archetype = GetArchetype(new_bitmap);

                if (!entity.components_types.IsNull()) {
                    Archetype* old_archetype = GetArchetype(entity.components_types);

                    int index = new_archetype->PopEntityFromOtherArchetype(entity, old_archetype);

                    entity.index = index;
                    entity.components_types = new_bitmap;
                }
            }
        }

        template<typename ...Types, typename F>
        void System(const F& callback) {
            // TODO: optimize this
            TypesBitmap types = (... | type_manager->GetTypeBitmap<Types>());

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
                    archetype->CallFunctionWithComponents<Types...>(callback);
                }
            }
        }
    };
}
