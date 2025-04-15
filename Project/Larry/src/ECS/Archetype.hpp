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

        public:
            TypesBitmap contained_types_bitmap;
            // containes 
            UnknownTypeVector entity_and_components;

            Archetype() : entity_and_components(sizeof(EncodedEntity)) {

            }

            Archetype(TypesBitmap contained_types_bitmap_, TypeManager* type_manager_) :
                contained_types_bitmap(contained_types_bitmap_),
                type_manager(type_manager_)
            {
                const int ENTITY_SIZE = sizeof(EncodedEntity);
                int index = ENTITY_SIZE;

                contained_types_bitmap.ForEachType([&](TypesBitmap curr){
                    type_mapper[curr] = index;
                    index += type_manager->GetTypeSize(curr);
                });

                entity_and_components = UnknownTypeVector(index);
            }

            const UnknownTypeTypeMapper* GetTypeMapper() {
                return &type_mapper;
            }

            int GetObjectSize() {
                return entity_and_components.GetElementDataSize();
            }

            int GetSize() {
                return entity_and_components.Size();
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

            template<typename ...Types, typename F>
            void CallFunctionWithComponents(const F& callback) {
                int size = entity_and_components.Size();
                for (int i = 0; i < size; i++) {
                    byte* data = entity_and_components.GetRawByIndex(i);
                    callback((Types&)(*(data+type_mapper[type_manager->GetTypeBitmap<Types>()]))...);
                }
            }

    };

}
