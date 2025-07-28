#pragma once

#include "ECS/ECS_C.h"
#include <vector>

namespace Larry::ECS::Internal {

    struct AnyQueue {
        std::vector<ECS_Any> elements;
        int index = 0;
        int pop_back_index;

        void Clear() {
            elements.clear();
            index = 0;
        }

        ECS_Any Pop() {
            ECS_Any res = elements[index];
            index++;
            return res;
        }

        void InitPopBack() {
            pop_back_index = elements.size() - 1;
        }

        ECS_Any PopBack() {
            ECS_Any res = elements[pop_back_index];
            pop_back_index--;
            return res;
        }
    };

    struct TypeQueue {
        std::vector<ECS_TypeHashCode> elements;

        void push_back(ECS_TypeHashCode code) {
            elements.push_back(code);
        }

        ECS_TypeHashCode& operator[](int i) {
            return elements[i];
        }

        void Clear() {
            elements.clear();
        }
    };

}

