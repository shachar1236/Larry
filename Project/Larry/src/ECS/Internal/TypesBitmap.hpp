#pragma once
#include <bitset>
#include <cstddef>
#include <cstring>
#include <functional>
#include <strings.h>

#define FIND_NEXT_CHUNK_SIZE 32


namespace Larry::ECS::Internal {

    /* using bitset = unsigned long long; */
    using bitset = std::bitset<256>;

    inline bitset bit(int i) {
        bitset ret;
        ret.set(i);
        return ret;
    }


    class TypesBitmap {
        public:
            static std::bitset<256> first32mask;

            static void Init();
            bitset bitmap;
            static const int MAX_TYPE_NUMBER = 256;

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

            TypesBitmap operator~() const {
                return TypesBitmap(~this->bitmap);
            }

            bool operator==(const TypesBitmap& other) const {
                return bitmap == other.bitmap;
            }

            bool operator==(int other) const {
                return bitmap == other;
            }

            bool IsNull() const {
                return bitmap == 0;
            }

            bool Intersect(TypesBitmap other) {
                return !(other & *this).IsNull();
            }

            static TypesBitmap TypeWithIndex(int i) {
                TypesBitmap ret;
                ret.bitmap.set(i);
                return ret;
            }

            int find_next(int prevPlusOne) const {
                const int CHUNK_NUMBER = MAX_TYPE_NUMBER / FIND_NEXT_CHUNK_SIZE;

                int prev = prevPlusOne - 1;
                int prev_chunk = prev / FIND_NEXT_CHUNK_SIZE;
                int prev_index = prev % FIND_NEXT_CHUNK_SIZE;

                int curr_chunk = prevPlusOne / FIND_NEXT_CHUNK_SIZE;
                int curr_index = prevPlusOne % FIND_NEXT_CHUNK_SIZE;

                int start_chunk = curr_chunk;
                int start_index = curr_index;
                if (prev != -1 && curr_index != 0) {
                    std::bitset<256> first32bits = ((bitmap >> (FIND_NEXT_CHUNK_SIZE * curr_chunk)) & first32mask);
                    bool curr_chunk_empty = ((~(first32mask >> (FIND_NEXT_CHUNK_SIZE - prev_index))) & first32bits) == 0;
                    if (curr_chunk_empty) {
                        start_chunk = curr_chunk+1;
                        start_index = 0;
                    }
                }

                for (int chunk = start_chunk; chunk < CHUNK_NUMBER; chunk++) {
                    std::bitset<256> first32bits = ((bitmap >> (FIND_NEXT_CHUNK_SIZE * chunk)) & first32mask);
                    bool chunk_not_empty = first32bits != 0;
                    if (chunk_not_empty) {
                        for (int i = start_index; i < FIND_NEXT_CHUNK_SIZE; i++) {
                            if ((bitmap & bit(chunk * FIND_NEXT_CHUNK_SIZE + i)) != 0) {
                                return i;
                            }
                        }
                    }
                    start_index = 0;
                }

                return -1;
            }

            inline int GetTypesCount() const {
                return bitmap.count();
            }

            template<typename F>
                void ForEachType(const F& callback) const {
                    int pos = find_next(0);
                    while (pos != -1) {
                        callback(TypeWithIndex(pos));
                        pos = find_next(pos+1);
                    }
                }
    };

}

using namespace Larry::ECS::Internal;
template <>
struct std::hash<TypesBitmap>
{
    std::size_t operator()(const TypesBitmap& k) const
    {
        return std::hash<bitset<256>>()(k.bitmap);;
    }
};
