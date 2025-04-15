#pragma once
#include "ECS/Entity.hpp"
#include "ECS/TypeManger.hpp"
#include "ECS/TypesBitmap.hpp"
#include "Log.h"
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
        const UnknownTypeTypeMapper* type_mapper;

        EntityData() {

        }

        EntityData(byte* data_, int size, TypeManager* type_manager_, const UnknownTypeTypeMapper* type_mapper_) : data(data_), data_size(size), type_manager(type_manager_), type_mapper(type_mapper_) {
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
        EntityData AddComponent(const T& component, const UnknownTypeTypeMapper* new_type_mapper) {
            LA_CORE_DEBUG("Adding one component to EntityData");

            byte* new_data = new byte[data_size + sizeof(T)];
            TypesBitmap type_bitmap = type_manager->GetTypeBitmap<T>();
            Entity* new_entity = (Entity*)new_data;

            auto index_it = new_type_mapper->find(type_bitmap);
            if (index_it != new_type_mapper->end()) {
                int index = index_it->second;
                memcpy(new_data, data, index);
                memcpy(new_data+index, &component, sizeof(T));
                memcpy(new_data+index+sizeof(T), data+index, data_size-index);
                new_entity->components_types = new_entity->components_types | type_bitmap;
                return EntityData(new_data, data_size+sizeof(T), type_manager, new_type_mapper);
            } else {
                LA_CORE_WARN("ECS: Type dosent exist on mapper!");
                return EntityData(nullptr, 0, type_manager, nullptr);
            }
        }

        // returns new EntityData with the new component.
        // WARNING! - old EntityData.data is not freed automaticly, you should handle it yourself
        template<typename... Types>
        EntityData AddComponent(const Types&... args, const UnknownTypeTypeMapper* new_type_mapper) {
            LA_CORE_DEBUG("Adding multipule component to EntityData");

            const int TYPES_SIZE = (... + sizeof(Types));
            byte* new_data = new byte[data_size + TYPES_SIZE];
            memcpy(new_data, data, sizeof(Entity));
            Entity* new_entity = (Entity*)new_data;

            int size = data_size;
            auto copy_argument_funtion = [&]<typename T>(const T& arg){
                TypesBitmap type_bitmap = type_manager->GetTypeBitmap<T>();
                auto index_it = new_type_mapper->find(type_bitmap);
                if (index_it != new_type_mapper->end()) {
                    memcpy(new_data+index_it->second, &arg, sizeof(T));
                    new_entity->components_types = new_entity->components_types | type_bitmap;
                } else {
                    LA_CORE_WARN("ECS: Type dosent exist on mapper!");
                }
                size += sizeof(T);
            };

            (copy_argument_funtion(args), ...);

            if (type_mapper != nullptr) {
                for (auto type : *type_mapper) {
                    memcpy(new_data + new_type_mapper->find(type.first)->second, data + type.second, type_manager->GetTypeSize(type.first));
                }
            }

            return EntityData(new_data, size, type_manager, new_type_mapper);
        }

        bool IsNull() {
            return data == nullptr;
        }
    };
}
