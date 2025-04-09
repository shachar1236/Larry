#pragma once

#include "EventSystem/Event.h"
#include "LarryMemory.h"
#include <cstdio>
#include <functional>

/* #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) */
/* #define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); } */
#define BIND_EVENT_FN(x) [this](const Ref<Event> e){ this->x(e); }
#define EVENT_LAMBDA(a, b) [a](const Ref<Event>& event){b}

namespace Larry {
    class EventSystem {
        public:
            /* static void(*CallbackFunction)(const Ref<Event>&); */
            using EventCallbackFunction = std::function<void(const Ref<Event>&)> ;
            static EventCallbackFunction CallbackFunction;

            static void SetCallbackFunction(const EventCallbackFunction& cl);

            static void HandleEvent(const Ref<Event>& event);
    };


    template<typename T, typename F>
    bool DispatchEvent(const Ref<Event>& event, const F& func) {
        if (event->GetEventType() == T::GetStaticType()) {
            func(event);
            return true;
        }
        return false;
    }

}
