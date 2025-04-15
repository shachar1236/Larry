#pragma once
#include "Log.h"
#include <cstddef>
#include <functional>
#include <utility>
#include <sul/dynamic_bitset.hpp>

namespace Larry::ECS {

    class TypesBitmap {
        public:
            static const int MAX_TYPE_NUMBER = 256;
            sul::dynamic_bitset<> bitmap;

            TypesBitmap() : bitmap(MAX_TYPE_NUMBER) {

            }

            TypesBitmap operator|(const TypesBitmap& other) const {
                TypesBitmap result;
                result.bitmap = this->bitmap | other.bitmap;
                return result;
            }

            TypesBitmap operator&(const TypesBitmap& other) const {
                TypesBitmap result;
                result.bitmap = this->bitmap & other.bitmap;
                return result;
            }

            TypesBitmap operator^(const TypesBitmap& other) const {
                TypesBitmap result;
                result.bitmap = this->bitmap ^ other.bitmap;
                return result;
            }

            bool operator==(const TypesBitmap& other) const {
                return this->bitmap == other.bitmap;
            }
            
            bool IsNull() {
                return bitmap.none();
            }

            static TypesBitmap TypeWithIndex(int i) {
                TypesBitmap ret;
                ret.bitmap.set(i);
                return ret;
            }

            /* int FindNext(int prev) {
                return bitmap.find_next(prev);
            } */

            inline int GetTypesCount() {
                return bitmap.count();
            }

            template<typename F>
            void ForEachType(const F& callback) {
                int pos = bitmap.find_first();
                while (pos != bitmap.npos) {
                    callback(TypeWithIndex(pos));
                    pos = bitmap.find_next(pos);
                }
            }
    };

}

template <>
struct std::hash<Larry::ECS::TypesBitmap>
{
    std::size_t operator()(const Larry::ECS::TypesBitmap& k) const
    {
        std::size_t res;
        auto data = k.bitmap.data();
        for (int i = 0; i < k.bitmap.num_blocks(); i++) {
            res = res ^ std::hash<unsigned long long>()(data[i]);
        }
        return res;
    }
};
