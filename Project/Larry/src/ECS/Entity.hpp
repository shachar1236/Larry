#pragma once

#include "ECS/TypesBitmap.hpp"
#include "Log.h"
#include "TypeManger.hpp"
#include <functional>
#include "UnknownTypeVector.hpp"
#include <optional>
#include <unordered_map>

namespace Larry::ECS {
    typedef unsigned int UID;

    class World;
    class Archetype;
    struct EntityData;
    class Entity;

    struct EncodedEntity {
        UID id;
        Entity* owner;

        bool operator==(const EncodedEntity& other) const {
            return id == other.id;
        }
    };

    class Entity {
        private:
            UID id;
            TypesBitmap components_types;
            TypeManager* type_manager;

            UnknownTypeTypeMapper* type_mapper;
            byte* data;

            std::function<void(EncodedEntity)> kill_callback;
            bool is_alive = true;

            friend World;
            friend EntityData;
            friend Archetype;
        public:
            Entity(UID id_, const std::function<void(EncodedEntity)>& kill_callback_, TypeManager* type_manger_) :
                id(id_),
                kill_callback(kill_callback_),
                type_manager(type_manger_)
            {
            }

            void Kill() {
                if (is_alive) {
                    is_alive = false;
                    kill_callback(EncodedEntity{id, this});
                }
            }

            ~Entity() {
                Kill();
            }

            bool IsAlive() const {
                return is_alive;
            }

            UID GetId() const {
                return id;
            }

            template<typename ...Types, typename F>
            void SetComponent(F set_callback) {
                if (is_alive) {
                    set_callback((Types&)(*(data+(*type_mapper)[type_manager->GetTypeBitmap<Types>()]))...);
                }
            }

            // returns component assossiate with T
            // IMPORTANT: the component should be read only, if you want to change its value you should use SetComponent
            template<typename T>
            std::optional<const T*> GetComponent() {
                if (is_alive) {
                    TypesBitmap type = type_manager->GetTypeBitmap<T>();
                    auto index_it = (*type_mapper).find(type);
                    if (index_it != type_mapper->end()) {
                        return (T*)(data+index_it->second);
                    }
                }
                return std::nullopt;
            } 

            bool operator==(const Entity& other) const {
                return id == other.id;
            }
    };

}
