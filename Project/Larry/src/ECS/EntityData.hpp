#pragma once
#include "ECS/Entity.hpp"
#include "ECS/TypeManger.hpp"
#include "ECS/TypesBitmap.hpp"
#include <cstring>

typedef unsigned char byte;

namespace Larry::ECS {

    struct EntityData {
    private:
        template<typename T>
        T* GetComponentFromRawIndex(int i) {
            return data[i];
        }
    public:
        byte* data;
        int data_size;
        Entity* entity;
        TypeManager* type_manager;

        EntityData() {

        }

        EntityData(byte* data_, int size, TypeManager* type_manager_) : data(data_), data_size(size), type_manager(type_manager_) {
            entity = (Entity*)data;
        }

        ~EntityData() {
        }

        template<typename T>
        T* GetComponent() {
            TypesBitmap curr;
            TypesBitmap type_bitmap = type_manager->GetTypeBitmap<T>();

            int number = 0;
            int size = sizeof(Entity);
            do {
                curr = entity->components_types.GetType(number);
                if (curr == type_bitmap) {
                    return GetComponentFromRawIndex<T>(size);
                }
                
                size += type_manager->GetTypeSize(curr);
                number++;
            } while (!curr.IsNull());
        }

        // returns new EntityData with the new component.
        // WARNING! - old EntityData.data is not freed automaticly, you should handle it yourself
        template<typename T>
        EntityData AddComponent(const T& component) {
            byte* new_data = new byte[data_size + sizeof(T)];
            TypesBitmap curr;
            TypesBitmap type_bitmap = type_manager->GetTypeBitmap<T>();

            int types_count = type_bitmap.GetTypesCount();

            if (types_count == 0) {
                memcpy(new_data, data, sizeof(Entity));
                memcpy(new_data + sizeof(Entity), &component, sizeof(T));
                Entity* new_entity = (Entity*)new_data;
                new_entity->components_types = type_bitmap;
                return EntityData(new_data, data_size+sizeof(T), type_manager);
            } else if (types_count == 1) {
                curr = entity->components_types.GetType(1);
                if (curr.Bitmap < type_bitmap.Bitmap) {
                    memcpy(new_data, data, data_size);
                    memcpy(new_data+data_size, &component, sizeof(T));
                } else {
                    memcpy(new_data, data, sizeof(Entity));
                    memcpy(new_data+sizeof(Entity), &component, sizeof(T));
                    memcpy(new_data+sizeof(Entity)+sizeof(T), data+sizeof(Entity), data_size-sizeof(Entity));
                }
                Entity* new_entity = (Entity*)new_data;
                new_entity->components_types = new_entity->components_types | type_bitmap;
                return EntityData(new_data, data_size+sizeof(T), type_manager);
            }

            int number = 1;
            int size = sizeof(Entity);
            curr = entity->components_types.GetType(number);
            while (!curr.IsNull()) {
                number++;
                TypesBitmap new_curr = entity->components_types.GetType(number);
                if (type_bitmap.Bitmap > curr.Bitmap && type_bitmap.Bitmap < new_curr.Bitmap) {
                    memcpy(new_data, data, size);
                    memcpy(new_data + size, (byte*)(&component), sizeof(T));
                    memcpy(new_data + size + sizeof(T), data+size, data_size - size);
                    Entity* new_entity = (Entity*)new_data;
                    new_entity->components_types = new_entity->components_types | type_bitmap;
                    return EntityData(new_data, data_size+sizeof(T), type_manager);
                }

                curr = new_curr;
                size += type_manager->GetTypeSize(curr);
                number++;
            } 
            return EntityData(nullptr, 0, type_manager);
        }

        bool IsNull() {
            return data == nullptr;
        }
    };
}
