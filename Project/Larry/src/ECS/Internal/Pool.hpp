#pragma once

#include <queue>

namespace Larry::ECS::Internal {

    template<typename T>
    class Pool {
        private:
            std::queue<T*> elements;
        public:
            Pool() {

            }

            ~Pool() {
                while (!elements.empty()) {
                    delete elements.back();
                    elements.pop();
                }
            }

            T* Get() {
                if (!elements.empty()) {
                    T* r = elements.front();
                    elements.pop();
                    return r;
                }
                
                return new T();
            }

            void Return(T* e) {
                elements.push(e);
            }
    };
}
