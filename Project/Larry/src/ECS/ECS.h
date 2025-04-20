#pragma once

#include <functional>
namespace Larry::ECS {
    void Init(); 
    using BreakFunction = std::function<void()>;
}
