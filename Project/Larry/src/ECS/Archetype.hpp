#pragma once
#include "UnknownTypeVector.hpp"
#include "ECS/Entity.hpp"
#include "TypesBitmap.hpp"
#include <functional>
#include <typeinfo>
#include <vector>

namespace Larry::ECS {

    class Archetype {
        public:
            TypesBitmap contained_types_bitmap;
            // containes 
            UnknownTypeVector entity_and_components;

            Archetype() : entity_and_components(sizeof(Entity)) {

            }

            Archetype(TypesBitmap contained_types_bitmap_, int contained_types_size) :
                contained_types_bitmap(contained_types_bitmap_),
                entity_and_components(sizeof(Entity) + contained_types_size) 
            {

            }

            int GetObjectSize() {
                return entity_and_components.GetElementDataSize();
            }

            void Add(const byte* data) {
                entity_and_components.AddRawData(data);
            }

            void PopEntityAndComponents(const Entity& entity, byte* dest) {
                int size = entity_and_components.Size();
                for (int i = 0; i < size; i++) {
                    Entity* curr_entity = entity_and_components.GetByIndex<Entity>(i);
                    if (entity == *curr_entity) {
                        entity_and_components.Pop(i, dest);
                        return;
                    }
                }
            }

            void AddData(const byte* data) {
                entity_and_components.AddRawData(data);
            }
    };

}
