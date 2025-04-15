#include "ECS/World.hpp"
#include <cassert>

namespace Larry::ECS {

struct _Position {
    float x, y;
};

struct _Velocity {
    float x, y;
};

struct _Transform {
    float x, y, z;
};

void TestECS()
{
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

    world.InsertComponent<_Position, _Velocity>(entity, { 1, 8 }, { 2, 9 });
    auto pos = entity->GetComponent<_Position>();
    assert(pos.has_value());
    assert(pos.value()->x == 1 && pos.value()->y == 8);
    auto vel = entity->GetComponent<_Velocity>();
    assert(vel.has_value());
    assert(vel.value()->x == 2 && vel.value()->y == 9);

    entity->SetComponent<_Position>([](_Position& position) { position.x = 0; });
    pos = entity->GetComponent<_Position>();
    assert(pos.has_value());
    assert(pos.value()->x == 0 && pos.value()->y == 8);
    vel = entity->GetComponent<_Velocity>();
    assert(vel.has_value());
    assert(vel.value()->x == 2 && vel.value()->y == 9);

    entity->SetComponent<_Position, _Velocity>([](_Position& position, _Velocity& velocity) {
        position.x = 5;
        velocity.y = 3;
    });
    pos = entity->GetComponent<_Position>();
    assert(pos.has_value());
    assert(pos.value()->x == 5 && pos.value()->y == 8);
    vel = entity->GetComponent<_Velocity>();
    assert(vel.has_value());
    assert(vel.value()->x == 2 && vel.value()->y == 3);

    world.InsertComponent<_Transform>(entity, { 1, 8, 6 });
    pos = entity->GetComponent<_Position>();
    assert(pos.has_value());
    assert(pos.value()->x == 5 && pos.value()->y == 8);
    vel = entity->GetComponent<_Velocity>();
    assert(vel.has_value());
    assert(vel.value()->x == 2 && vel.value()->y == 3);
    auto trans = entity->GetComponent<_Transform>();
    assert(trans.has_value());
    assert(trans.value()->x == 1 && trans.value()->y == 8 && trans.value()->z == 6);
}
}
