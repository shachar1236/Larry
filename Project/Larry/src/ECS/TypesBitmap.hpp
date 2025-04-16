#pragma once
#include "Log.h"
#include <cstddef>
#include <cstring>
#include <functional>
#include <strings.h>
#include <sul/dynamic_bitset.hpp>

#define BITMAP_CHUNK_SIZE 4

int countSetBits(unsigned long long n) 
{ 
    unsigned int count = 0; 
    while (n) 
    { 
      n &= (n-1) ; 
      count++; 
    } 
    return count; 
} 

namespace Larry::ECS {
    using bitset = unsigned long long;

    inline bitset bit(int i) {
        return (bitset)1 << i;
    }

    class TypesBitmap {
        // TODO: currenty support up to 64 different types, make it support up to 256
        public:
            bitset bitmap;
            static const int MAX_TYPE_NUMBER = sizeof(bitset) * 8 * BITMAP_CHUNK_SIZE;

            TypesBitmap() {
                bitmap = 0;
            }

            TypesBitmap(const TypesBitmap& other) {
                bitmap = other.bitmap;
            }

            TypesBitmap(bitset bits) {
                bitmap = bits;
            }

            TypesBitmap operator|(const TypesBitmap& other) const {
                return TypesBitmap(this->bitmap | other.bitmap);
            }

            TypesBitmap operator&(const TypesBitmap& other) const {
                return TypesBitmap(this->bitmap & other.bitmap);
            }

            bool operator==(const TypesBitmap& other) const {
                return bitmap == other.bitmap;
            }
            
            bool IsNull() {
                return bitmap == 0;
            }

            static TypesBitmap TypeWithIndex(int i) {
                return TypesBitmap(bit(i));
            }

            int find_next(int prevPlusOne) {
                for (int i = prevPlusOne; i < sizeof(bitset) * 8; i++) {
                    if ((bitmap & bit(i)) != 0) {
                        return i;
                    }
                }

                return -1;
            }

            inline int GetTypesCount() {
                return countSetBits(bitmap);
            }

            template<typename F>
            void ForEachType(const F& callback) {
                int pos = find_next(0);
                while (pos != -1) {
                    callback(TypeWithIndex(pos));
                    pos = find_next(pos+1);
                }
            }
    };

}

template <>
struct std::hash<Larry::ECS::TypesBitmap>
{
    std::size_t operator()(const Larry::ECS::TypesBitmap& k) const
    {
        return std::hash<unsigned long long>()(k.bitmap);;
    }
};
