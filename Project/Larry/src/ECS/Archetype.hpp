#pragma once
#include "LarryMemory.h"
#include "TypeManger.hpp"
#include "UnknownTypeVector.hpp"
#include "ECS/Entity.hpp"
#include "ECS/EntityData.hpp"
#include "TypesBitmap.hpp"
#include <strings.h>

namespace Larry::ECS {

    class Archetype {
        private:
            TypeManager* type_manager;
            UnknownTypeTypeMapper type_mapper;

            void GenerateTypeMapper() {
                int number = 1;
                TypesBitmap curr = contained_types_bitmap.GetType(number);
                const int ENTITY_SIZE = sizeof(EncodedEntity);
                int index = ENTITY_SIZE;
                while (!curr.IsNull()) {
                    type_mapper[curr] = index;
                    index += type_manager->GetTypeSize(curr);

                    number++;
                    curr = contained_types_bitmap.GetType(number);
                }
            }

        public:
            TypesBitmap contained_types_bitmap;
            // containes 
            UnknownTypeVector entity_and_components;

            Archetype() : entity_and_components(sizeof(EncodedEntity)) {

            }

            Archetype(TypesBitmap contained_types_bitmap_, int contained_types_size, TypeManager* type_manager_) :
                contained_types_bitmap(contained_types_bitmap_),
                entity_and_components(sizeof(EncodedEntity) + contained_types_size),
                type_manager(type_manager_)
            {
                GenerateTypeMapper();
            }

            const UnknownTypeTypeMapper* GetTypeMapper() {
                return &type_mapper;
            }

            int GetObjectSize() {
                return entity_and_components.GetElementDataSize();
            }

            void Add(const EntityData& data, const Ref<Entity>& entity) {
                byte* new_data = entity_and_components.AddRawData(data.data);
                entity->type_mapper = &type_mapper;
                entity->data = new_data;
            }

            void PopEntityAndComponents(const EncodedEntity& entity, byte* dest) {
                int size = entity_and_components.Size();
                for (int i = 0; i < size; i++) {
                    EncodedEntity* curr_entity = entity_and_components.GetByIndex<EncodedEntity>(i);
                    if (entity == *curr_entity) {
                        entity_and_components.Pop(i, dest);
                        return;
                    }
                }
            }

    };

}
