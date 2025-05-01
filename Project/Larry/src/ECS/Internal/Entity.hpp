#pragma once

#include "ECS/Internal/TypesBitmap.hpp"

namespace Larry::ECS::Internal {

    typedef unsigned long long UID;

    class World;
    class Archetype;
    struct EntityData;
    class Entity;

    struct EncodedEntity {
        UID id;
        bool alive = true;

        bool operator==(const EncodedEntity& other) const {
            return id == other.id;
        }
    };

    class Entity {
        private:
            UID id;
            TypesBitmap components_types = {0};
            int index = -1;
            bool alive = true;

            friend World;
            friend EntityData;
            friend Archetype;

            Entity(UID id_) :
                id(id_)
        {
        }
        public:
            Entity() {
                alive = false;
            }

            ~Entity() {
            }

            EncodedEntity ToEncodedEntity() const {
                return {this->id, alive};
            }

            bool IsAlive() const {
                return alive;
            }

            UID GetId() const {
                return id;
            }

            int GetIndex() {
                return index;
            }
    };
}
