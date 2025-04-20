#pragma once
#include "ECS/TypeManager.hpp"
#include "ECS/TypesBitmap.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Archetype.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <optional>
#include <unordered_set>
#include "ECS/Utils.hpp"

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

        TypesBitmap singeltons_bitmap;
        std::unordered_map<TypesBitmap, byte*> singeltons;

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

            for (auto& [_, ptr] : singeltons) {
                delete[] ptr;
            }
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
            if (archetype->IsAlive(entity)) {
                archetype->KillEntity(entity);
            }
        }

        Entity CreateEntity() {
            UID id = FamilyGenerate();
            Entity entity = Entity(id);
            return entity;
        }

        template<typename T, typename F>
        void CreateSingelton(const F& callback) {
            TypesBitmap type = type_manager->GetTypeBitmap<T>();
            if (singeltons.find(type) == singeltons.end()) {
                singeltons[type] = new byte[sizeof(T)];
                singeltons_bitmap = singeltons_bitmap | type;
                callback((T&)(*singeltons[type]));
            }
        }

        template<typename T>
        std::optional<T*> GetSingelton() {
            TypesBitmap type = type_manager->GetTypeBitmap<T>();
            auto res = singeltons.find(type) ;
            if (res != singeltons.end()) {
                return (T*)(res->second);
            }
            return std::nullopt;
        }
        
        template<typename T, typename F>
        void SetSingelton(const F& callback) {
            TypesBitmap type = type_manager->GetTypeBitmap<T>();
            callback((T&)(*singeltons[type]));
        }

        // Inserts component to entity
        // return - if completed successfully
        template<typename ...Types, typename F>
        bool InsertComponent(Entity& entity, const F& set_callcack) {
            TypesBitmap new_bitmap = entity.components_types | (type_manager->GetTypeBitmap<Types>() | ...);
            bool type_in_singeltons = !(new_bitmap & singeltons_bitmap).IsNull();
            if (entity.alive && !type_in_singeltons) {
                Archetype* new_archetype = GetArchetype(new_bitmap);

                if (!entity.components_types.IsNull()) {
                    Archetype* old_archetype = GetArchetype(entity.components_types);
                    if (old_archetype->IsAlive(entity)) {
                        int index = new_archetype->PopEntityFromOtherArchetype(entity, old_archetype);
                        new_archetype->SetComponents<Types...>(index, set_callcack);

                        entity.index = index;
                    } else {
                        entity.alive = false;
                        return false;
                    }
                } else {
                    int index = new_archetype->AllocateNew(entity);
                    new_archetype->SetComponents<Types...>(index, set_callcack);

                    entity.index = index;
                }

                // update entity to match the new data
                entity.components_types = new_bitmap;
                return true;
            }
            return false;
        }

        template<typename ...Types, typename F>
        void SetComponents(const Entity& entity, const F& set_callcack) {
            if (entity.alive) {
                Archetype* archetype = GetArchetype(entity.components_types);
                if (archetype->IsAlive(entity)) {
                    archetype->SetComponents<Types...>(entity.index, set_callcack);
                }
            }
        }

        template<typename T>
        std::optional<const T*> GetComponent(const Entity& entity) {
            if (entity.alive) {
                Archetype* archetype = GetArchetype(entity.components_types);
                if (archetype->IsAlive(entity)) {
                    return archetype->GetComponent<T>(entity);
                }
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
                    if (old_archetype->IsAlive(entity)) {
                        int index = new_archetype->PopEntityFromOtherArchetype(entity, old_archetype);

                        entity.index = index;
                        entity.components_types = new_bitmap;
                    }

                }
            }
        }

    private:
        template<typename ...Types, typename F>
        void _systemImpl(
                TypeWithHidden<TypesBitmap, Types>... types,
                TypeWithHidden<byte*, Types>... singeltons,
                TypesBitmap types_bitmap,
                const std::unordered_set<Archetype*>& my_archetypes,
                const F& callback)
        {
            for (auto& archetype : my_archetypes) {
                bool has_types = (archetype->GetTypesBitmap() & types_bitmap) == types_bitmap;
                if (has_types) {
                    archetype->CallFunctionWithComponents<Types...>(types..., singeltons..., singeltons_bitmap, callback);
                }
            }
        }

    public:
        template<typename ...Types, typename F>
        void System(const F& callback) {
            // TODO: optimize this
            TypesBitmap types = (~singeltons_bitmap) & (... | type_manager->GetTypeBitmap<Types>());

            int shortest = -1;
            std::unordered_set<Archetype*> my_archetypes;
            types.ForEachType([&](TypesBitmap curr){
                std::unordered_set<Archetype*> c = type_to_archetypes[curr];
                if (c.size() < my_archetypes.size() || shortest == -1) {
                    my_archetypes = c;
                }
            });

            _systemImpl<Types...>({type_manager->GetTypeBitmap<Types>()}...,
                    {singeltons.find(type_manager->GetTypeBitmap<Types>()) != singeltons.end() ? singeltons[type_manager->GetTypeBitmap<Types>()] : nullptr}...,
                    types,
                    my_archetypes,
                    callback);
        }
    };
}
