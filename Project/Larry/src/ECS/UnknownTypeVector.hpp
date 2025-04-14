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
            UnknownTypeVector(int element_data_size_) : element_data_size(element_data_size_) {
                
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

            inline void AddRawData(const byte* component_data) {
                /* int last_data_size = data.size(); */
                /* byte* last_data = data.data(); */
                /* data.resize(last_data_size + data_size); */
                /* memcpy(last_data, component_data, data_size); */

                data.reserve(element_data_size);
                for (int i = 0; i < element_data_size; i++) {
                    data.push_back(component_data[i]);
                }
            }

            template<typename T>
            inline void AddData(const T& component) {
                AddRawData((byte*)(&component));
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
    };
}
