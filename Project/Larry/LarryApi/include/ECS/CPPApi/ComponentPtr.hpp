#pragma once

#include "ECS/Internal/World.hpp"

#define TypeHash(T) typeid(T).hash_code()

namespace Larry::ECS {

    class World;

    template<typename T>
    class ComponentPtr {

        friend World;

        private:
            bool null;

            int* world_state;
            int last_world_state;

            T* ptr;
            Internal::World* world;
            ECS_Entity entity;

            ComponentPtr(ECS_Entity entity_, Internal::World* world_, int* world_state_) :
                entity(entity_),
                world(world_),
                world_state(world_state_),
                null(false),
                ptr(nullptr)
            {
                last_world_state = *world_state - 1;
            }

            void update() {
                if (!null && last_world_state == *world_state) {
                    return;
                }

                std::optional<ECS_Any> res = world->GetComponent(entity, TypeHash(T));
                if (res.has_value()) {
                    ptr = (T*)res->value;
                } else {
                    ptr = nullptr;
                }

                last_world_state = *world_state;
            }
        public:

            ComponentPtr() : null(true), ptr(nullptr) {}

            T& operator*() {
                update();
                return *ptr;
            }

            T* operator->() {
                update();
                return ptr;
            }

            bool has_value() {
                update();
                return ptr != nullptr;
            }
    };
}
