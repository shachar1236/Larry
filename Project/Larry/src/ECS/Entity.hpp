#pragma once

#include "ECS/TypesBitmap.hpp"

namespace Larry::ECS {
    typedef unsigned int UID;

    class World;
    struct EntityData;

    class Entity {
        private:
            UID id;
            TypesBitmap components_types;

            friend World;
            friend EntityData;
        public:
            Entity(UID id_) : id(id_) {
                components_types = {0};
            }

            ~Entity() {}

            UID GetId() {
                return id;
            }

            bool operator==(const Entity& other) const {
                return id == other.id;
            }
    };
}
