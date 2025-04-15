#pragma once

#include "ECS/TypesBitmap.hpp"
#include <cstddef>
#include <cstdio>
#include <unordered_map>
namespace Larry::ECS {

    using UnknownTypeTypeMapper = std::unordered_map<TypesBitmap, int>;
    using TypeHashCode = size_t;

    class TypeManager {
    private:
        int last_type_index;
        std::unordered_map<TypeHashCode, TypesBitmap> typeHash_to_TypeBitmap;

        std::unordered_map<TypesBitmap, int> typeBitmap_to_size;

        static TypeManager* instance;

    public:
        TypeManager() { }

        template <typename T> TypesBitmap GetTypeBitmap()
        {
            TypeHashCode hash = typeid(T).hash_code();
            auto res = typeHash_to_TypeBitmap.find(hash);
            if (res == typeHash_to_TypeBitmap.end()) {
                // new type
                TypesBitmap type = TypesBitmap::TypeWithIndex(last_type_index);
                last_type_index++;
                typeHash_to_TypeBitmap[hash] = type;
                typeBitmap_to_size[type] = sizeof(T);
                return type;
            } else {
                return res->second;
            }
        }

        template <typename T> constexpr void RegisterType() { GetTypeBitmap<T>(); }

        int GetTypeSize(TypesBitmap type) { return typeBitmap_to_size[type]; }
    };
}
