#pragma once
#include "ECS/Entity.hpp"
#include "ECS/TypeManger.hpp"
#include "ECS/TypesBitmap.hpp"
#include "Log.h"
#include <cassert>
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
        EncodedEntity* encoded_entity;
        TypesBitmap components_types;
        TypeManager* type_manager;
        const UnknownTypeTypeMapper* type_mapper;

        EntityData() {

        }

        EntityData(byte* data_, int size, TypesBitmap components_types_, TypeManager* type_manager_, const UnknownTypeTypeMapper* type_mapper_) :
            data(data_),
            data_size(size),
            components_types(components_types_),
            type_manager(type_manager_),
            type_mapper(type_mapper_) 
        {
            encoded_entity = (EncodedEntity*)data;
        }

        ~EntityData() {
        }

        template<typename T>
        T* GetComponent() {
            TypesBitmap curr;
            TypesBitmap type_bitmap = type_manager->GetTypeBitmap<T>();

            auto it = type_mapper->find(type_bitmap);
            assert(it != type_mapper->end());
            return GetComponentFromRawIndex<T>(it->second);
        }

        // returns new EntityData with the new component.
        // WARNING! - old EntityData.data is not freed automaticly, you should handle it yourself
        template<typename T>
        EntityData AddComponent(const T& component, const UnknownTypeTypeMapper* new_type_mapper) {
            byte* new_data = new byte[data_size + sizeof(T)];
            TypesBitmap type_bitmap = type_manager->GetTypeBitmap<T>();
            EncodedEntity* new_entity = (EncodedEntity*)new_data;

            auto index_it = new_type_mapper->find(type_bitmap);
            if (index_it != new_type_mapper->end()) {
                int index = index_it->second;
                memcpy(new_data, data, index);
                memcpy(new_data+index, &component, sizeof(T));
                memcpy(new_data+index+sizeof(T), data+index, data_size-index);
                return EntityData(new_data, data_size+sizeof(T), components_types | type_bitmap, type_manager, new_type_mapper);
            } else {
                LA_CORE_WARN("ECS: Type dosent exist on mapper!");
                return EntityData(nullptr, 0, TypesBitmap(), type_manager, nullptr);
            }
        }

        // returns new EntityData with the new component.
        // WARNING! - old EntityData.data is not freed automaticly, you should handle it yourself
        template<typename... Types>
        EntityData AddComponent(const Types&... args, const UnknownTypeTypeMapper* new_type_mapper) {
            const int TYPES_SIZE = (... + sizeof(Types));
            byte* new_data = new byte[data_size + TYPES_SIZE];
            memcpy(new_data, data, sizeof(EncodedEntity));
            EncodedEntity* new_entity = (EncodedEntity*)new_data;
            TypesBitmap new_components_types = components_types;

            int size = data_size;
            auto copy_argument_funtion = [&]<typename T>(const T& arg){
                TypesBitmap type_bitmap = type_manager->GetTypeBitmap<T>();
                auto index_it = new_type_mapper->find(type_bitmap);
                if (index_it != new_type_mapper->end()) {
                    memcpy(new_data+index_it->second, &arg, sizeof(T));
                    new_components_types = new_components_types | type_bitmap;
                } else {
                    assert("ECS: Type dosent exist on mapper!");
                }
                size += sizeof(T);
            };

            (copy_argument_funtion(args), ...);

            if (type_mapper != nullptr) {
                for (auto type : *type_mapper) {
                    memcpy(new_data + new_type_mapper->find(type.first)->second, data + type.second, type_manager->GetTypeSize(type.first));
                }
            }

            return EntityData(new_data, size, new_components_types, type_manager, new_type_mapper);
        }

        bool IsNull() {
            return data == nullptr;
        }
    };
}
