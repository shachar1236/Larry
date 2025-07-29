#pragma once

#include "EventSystem/Event.h"
#include "LarryMemory.h"
#include <cstdio>
#include <functional>
#include <queue>
#include <vector>

/* #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) */
/* #define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); } */
#define BIND_EVENT_FN(x) [&](Event* e){ this->x(e); }
#define EVENT_LAMBDA(a, b) [a](Event* event){b}

namespace Larry {
    class EventSystem {
        public:
            /* static void(*CallbackFunction)(const Ref<Event>&); */
            using EventCallbackFunction = std::function<void(Event*)> ;
            static std::vector<EventCallbackFunction> CallbackFunctions;
            static std::queue<Event*> event_queue;

            static void AddCallbackFunction(const EventCallbackFunction& cl);

            static void HandleEvent(Event* event);
            static void HandleQueuedEvents();

    };


}
