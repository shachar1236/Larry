#include "ECS/World.hpp"

namespace Larry::ECS {

    struct _Position {
        float x, y;
    };

    struct _Velocity {
        float x, y;
    };

    struct _Transform {
        float x, y;
    };
    void TestECS() {
        /*  world ecs;

            ecs.system<Position, const Velocity>()
            .each([](Position& p, const Velocity& v) {
            p.x += v.x;
            p.y += v.y;
            });

            auto e = ecs.entity();

            ecs.insertComponent(e, [](Position& p, Velocity& v) {
            p = {10, 20};
            v = {1, 2};
            }); */
        World world;

        auto entity = world.CreateEntity();
        world.InsertComponent<_Position>(entity, {1, 8});
        world.InsertComponent<_Velocity>(entity, {1, 8});
        world.InsertComponent<_Transform>(entity, {1, 8});
    }
}
