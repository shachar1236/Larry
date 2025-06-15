#pragma once

#include "World.hpp"
namespace Larry::Scripts {
    class LuaScript {
        public:
            static void Init(ECS::Internal::World*, ECS_TypeHashCode);
    };
}
