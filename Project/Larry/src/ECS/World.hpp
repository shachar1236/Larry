#pragma once
#include "ECS/TypeManger.hpp"
#include "ECS/TypesBitmap.hpp"
#include "Entity.hpp"
#include "EntityData.hpp"
#include "Archetype.hpp"
#include "LarryMemory.h"
#include "Log.h"
#include <cstring>
#include <functional>
#include <typeinfo>
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

        Archetype* GetArchetype(TypesBitmap types) {
            auto res = archetypes.find(types);
            if (res != archetypes.end()) {
                return &(res->second);
            }

            archetypes[types] = Archetype(types, type_manager);
            return &(archetypes.find(types)->second);
        }
    public:
        World() {
            type_manager = new TypeManager();
        }

        ~World() {
            delete type_manager;
        }

        // kills an entity
        void KillEntity(EncodedEntity entity) {
            // TODO: create this funtion
        }

        Ref<Entity> CreateEntity() {
            UID id = FamilyGenerate();
            return CreateRef<Entity>(id, [this](EncodedEntity entity){ KillEntity(entity); }, type_manager);
        }

        
        // should return an entity with the given id
        // WARNING: an expensive function avoid if possible
        Ref<Entity> GetEntity(UID entity_id) {
            // TODO: create this function
            return nullptr;
        }
        
        template<typename ...Types>
        void InsertComponent(const Ref<Entity>& entity, const Types&... components) {
            EntityData new_entity_object;
            TypesBitmap new_bitmap = entity->components_types | (type_manager->GetTypeBitmap<Types>() | ...);
            Archetype* new_archetype = GetArchetype(new_bitmap);
            EncodedEntity enc_entity = {entity->id, entity.get()};

            if (!entity->components_types.IsNull()) {
                Archetype* archetype = GetArchetype(entity->components_types);

                byte* old_entity_data = new byte[archetype->GetObjectSize()];
                archetype->PopEntityAndComponents(enc_entity, old_entity_data);

                EntityData old_entity_object(old_entity_data, archetype->GetObjectSize(), entity->components_types, type_manager, archetype->GetTypeMapper());
                new_entity_object = old_entity_object.AddComponent<Types...>(components..., new_archetype->GetTypeMapper());

                delete[] old_entity_data;
            } else {
                EntityData old_entity_object = EntityData((byte*)&enc_entity, sizeof(EncodedEntity), entity->components_types, type_manager, nullptr);
                new_entity_object = old_entity_object.AddComponent<Types...>(components..., new_archetype->GetTypeMapper());

            }
            
            // update entity to match the new data
            entity->components_types = new_bitmap;
            new_archetype->Add(new_entity_object, entity);

            delete[] new_entity_object.data;
        }

        template<typename T>
        void DeleteComponent(const Ref<Entity> entity) {
            // TODO: create this function
        }

        template<typename ...Types, typename F>
        void System(const F& callback) {
            // TODO: optimize this
            TypesBitmap types = (... | type_manager->GetTypeBitmap<Types>());
            
            for (auto archetype : archetypes) {
                bool has_types = (archetype.first & types) == types;
                if (has_types) {
                    archetype.second.CallFunctionWithComponents<Types...>(callback);
                }
            }
        }
    };
}
