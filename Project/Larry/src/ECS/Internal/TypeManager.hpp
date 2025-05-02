#pragma once

#include "ECS/Internal/TypesBitmap.hpp"
#include "ECS_C.h"
#include "Internal/Queues.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <unordered_map>

#define DESTRUCTOR_LAMBDA(T) [](const void* x){ static_cast<const T*>(x)->~T(); }

namespace Larry::ECS::Internal {

    using UnknownTypeTypeMapper = std::unordered_map<TypesBitmap, int>;
    using TypeHashCode = size_t;

    class TypeManager {
        private:
            int last_type_index = 0;
            std::unordered_map<TypeHashCode, TypesBitmap> typeHash_to_TypeBitmap;

            std::unordered_map<TypesBitmap, int> typeBitmap_to_size;

            std::unordered_map<TypesBitmap, void(*)(const void*)> type_to_destructor;
        public:
            TypeManager() { }

            void RegisterType(ECS_TypeHashCode hash, int type_size, void(*destructor)(const void*)) {
                // new type
                TypesBitmap type = TypesBitmap::TypeWithIndex(last_type_index);
                last_type_index++;

                typeHash_to_TypeBitmap[hash] = type;

                typeBitmap_to_size[type] = type_size;

                if (destructor != NULL) {
                    type_to_destructor[type] = destructor;
                }
            }

            inline void RegisterTypeIfDosentExists(ECS_TypeHashCode hash, int type_size, void(*destructor)(const void*)) {
                if (typeHash_to_TypeBitmap.find(hash) == typeHash_to_TypeBitmap.end()) {
                    RegisterType(hash, type_size, destructor);
                }
            }

            TypesBitmap GetTypeBitmap(ECS_TypeHashCode hash)
            {
                auto res = typeHash_to_TypeBitmap.find(hash);
                assert(res != typeHash_to_TypeBitmap.end());
                return res->second;
            }

            int GetTypeSize(TypesBitmap type) { 
                auto res = typeBitmap_to_size.find(type);
                if (res == typeBitmap_to_size.end()) {
                    assert("Didnt find type size");
                }
                return res->second;
            }

            inline int GetTypeSize(ECS_TypeHashCode hash) { 
                TypesBitmap type = GetTypeBitmap(hash);
                return GetTypeSize(type);
            }

            void DestructType(TypesBitmap type, void* memory) {
                type_to_destructor[type](memory);
            }

            TypesBitmap QueueTypes(const AnyQueue& queue) {
                TypesBitmap t;
                for (auto& type : queue.elements) {
                    t = t | GetTypeBitmap(type.type);
                }
                return t;
            }

            TypesBitmap QueueTypes(const TypeQueue& queue) {
                TypesBitmap t;
                for (auto& type : queue) {
                    t = t | GetTypeBitmap(type);
                }
                return t;
            }
    };
}
