#pragma once
#include <cstring>
#include <vector>

typedef unsigned char byte;

namespace Larry::ECS {

    class UnknownTypeVector {
        private:
            std::vector<byte> data;
            int element_data_size;
        public:
            UnknownTypeVector() : element_data_size(0) {
            }

            UnknownTypeVector(int element_data_size_) : element_data_size(element_data_size_) {
                data.reserve(element_data_size * 4);
            }

            inline int SizeBytes() {
                return data.size();
            }

            inline int Size() {
                return data.size() / element_data_size;
            }

            inline int GetElementDataSize() {
                return element_data_size;
            }

            inline byte* AllocateData() {
                int last_data_size = data.size();
                byte* last_data = data.data() + last_data_size;
                data.resize(last_data_size + element_data_size);
                return last_data;
            }

            // returns data address
            inline byte* AddRawData(const byte* element_data) {
                int last_data_size = data.size();
                byte* last_data = data.data() + last_data_size;
                data.resize(last_data_size + element_data_size);
                memcpy(last_data, element_data, element_data_size);
                return last_data;
            }

            template<typename T>
            inline void AddData(const T& element) {
                AddRawData((byte*)(&element));
            }

            inline byte* GetRawByIndex(int i) {
                return data.data() + (i * element_data_size);
            }

            template<typename T>
            inline T* GetByIndex(int i) {
                return (T*)GetRawByIndex(i);
            }

            // deletes by index
            inline void Delete(int i) {
                int real = i * element_data_size;
                auto start = data.begin() + real;
                data.erase(start, start + element_data_size);
            }

            // pop by index
            inline void Pop(int i, byte* dest) {
                int real = i * element_data_size;
                memcpy(dest, data.data() + real, element_data_size);
                auto start = data.begin() + real;
                data.erase(start, start + element_data_size);
            }

            inline void Copy(int i, byte* dest) {
                int real = i * element_data_size;
                memcpy(dest, data.data() + real, element_data_size);
            }

            // sets element to 0
            inline void Clear(int index) {
                int real = index * element_data_size;
                memset(data.data() + real, 0, element_data_size);
            }
    };
}
