#pragma once
#include "ECS/TypeManger.hpp"
#include "ECS/TypesBitmap.hpp"
#include "Entity.hpp"
#include "EntityData.hpp"
#include "Archetype.hpp"
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

            TypesBitmap curr = types.GetType(1);
            int number = 2;
            int size = 0;
            while (!curr.IsNull()) {
                size += type_manager->GetTypeSize(curr);
                curr = types.GetType(number);
                number++;
            }

            archetypes[types] = Archetype(types, size, type_manager);
            return &(archetypes.find(types)->second);
        }
    public:
        World() {
            type_manager = new TypeManager();
        }

        ~World() {
            delete type_manager;
        }

        Entity CreateEntity() {
            return Entity(FamilyGenerate());
        }
        
        template<typename ...Types>
        void InsertComponent(Entity& entity, const Types&... components) {
            EntityData new_entity_object;
            TypesBitmap new_bitmap = entity.components_types | (type_manager->GetTypeBitmap<Types>() | ...);
            Archetype* new_archetype = GetArchetype(new_bitmap);

            if (!entity.components_types.IsNull()) {
                Archetype* archetype = GetArchetype(entity.components_types);

                byte* old_entity_data = new byte[archetype->GetObjectSize()];
                archetype->PopEntityAndComponents(entity, old_entity_data);

                EntityData old_entity_object(old_entity_data, archetype->GetObjectSize(), type_manager, archetype->GetTypeMapper());
                new_entity_object = old_entity_object.AddComponent<Types...>(components..., new_archetype->GetTypeMapper());

                entity = *new_entity_object.entity;
                delete[] old_entity_data;
            } else {
                EntityData old_entity_object = EntityData((byte*)&entity, sizeof(Entity), type_manager, nullptr);
                new_entity_object = old_entity_object.AddComponent<Types...>(components..., new_archetype->GetTypeMapper());

                entity = *new_entity_object.entity;
            }

            new_archetype->Add(new_entity_object);
            delete[] new_entity_object.data;
        }
    };
}
