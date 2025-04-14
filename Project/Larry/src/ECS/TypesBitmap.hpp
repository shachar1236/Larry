#pragma once
#include <cstddef>
#include <functional>
#include <utility>

int countSetBits(unsigned long n) 
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

    // TODO: currently supports up to 64 differnt types, we need to make it suppord unlimited types
    struct TypesBitmap {
        unsigned long Bitmap;
        static const int MAX_TYPE_NUMBER = sizeof(unsigned long);
        
        TypesBitmap operator|(TypesBitmap other) const {
            return {Bitmap | other.Bitmap};
        }

        TypesBitmap  operator&(TypesBitmap other) const {
            return {Bitmap & other.Bitmap};
        }

        bool operator==(TypesBitmap other) const {
            return Bitmap == other.Bitmap;
        }

        bool IsNull() {
            return Bitmap == 0;
        }

        static TypesBitmap TypeWithIndex(int i) {
            unsigned long res = 1 << i;
            return {res};
        }

        // returns each type that in the bitmap by its number in the bit map, 1 will return the first, 2 will return the scond ....
        TypesBitmap GetType(int number) {
            int count = 0;
            for (int i = 0; i < MAX_TYPE_NUMBER; i++) {
                TypesBitmap curr = TypeWithIndex(i);
                if (!(curr & *this).IsNull()) {
                    count++;
                    if (count == number) {
                        return curr;
                    }
                }
            }
            return {0};
        }

        int GetTypesCount() {
            return countSetBits(this->Bitmap);
        }

    };

}

template <>
struct std::hash<Larry::ECS::TypesBitmap>
{
  std::size_t operator()(const Larry::ECS::TypesBitmap& k) const
  {
    return std::hash<unsigned long>()(k.Bitmap);
  }
};
