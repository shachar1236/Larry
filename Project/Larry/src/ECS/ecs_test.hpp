#include "ECS/World.hpp"
#include "Log.h"
#include "TypesBitmap.hpp"
#include <cassert>

namespace Larry::ECS {

#ifdef LARRY_ENABLE_TESTING
    struct _Position {
        float x, y;
    };

    struct _Velocity {
        float x, y;
    };

    struct _Transform {
        float x, y, z;
    };

    void _InsertAndSetTest() {
        World world;

        auto entity = world.CreateEntity();

        world.InsertComponent<_Position, _Velocity>(entity, [&](_Position& pos, _Velocity& vel) {
                pos = { 1, 8 };
                vel = { 2, 9 };
        });
        auto pos = world.GetComponent<_Position>(entity);
        assert(pos.has_value());
        assert(pos.value()->x == 1 && pos.value()->y == 8);
        auto vel = world.GetComponent<_Velocity>(entity);
        assert(vel.has_value());
        assert(vel.value()->x == 2 && vel.value()->y == 9);

        world.SetComponents<_Position>(entity, [](_Position& position) { position.x = 0; });
        pos = world.GetComponent<_Position>(entity);
        assert(pos.has_value());
        assert(pos.value()->x == 0 && pos.value()->y == 8);
        vel = world.GetComponent<_Velocity>(entity);
        assert(vel.has_value());
        assert(vel.value()->x == 2 && vel.value()->y == 9);

        world.SetComponents<_Position, _Velocity>(entity, [](_Position& position, _Velocity& velocity) {
            position.x = 5;
            velocity.y = 3;
        });
        pos = world.GetComponent<_Position>(entity);
        assert(pos.has_value());
        assert(pos.value()->x == 5 && pos.value()->y == 8);
        vel = world.GetComponent<_Velocity>(entity);
        assert(vel.has_value());
        assert(vel.value()->x == 2 && vel.value()->y == 3);

        world.InsertComponent<_Transform>(entity, [](_Transform& trans){
            trans = { 1, 8, 6 };
        });
        pos = world.GetComponent<_Position>(entity);
        assert(pos.has_value());
        assert(pos.value()->x == 5 && pos.value()->y == 8);
        vel = world.GetComponent<_Velocity>(entity);
        assert(vel.has_value());
        assert(vel.value()->x == 2 && vel.value()->y == 3);
        auto trans = world.GetComponent<_Transform>(entity);
        assert(trans.has_value());
        assert(trans.value()->x == 1 && trans.value()->y == 8 && trans.value()->z == 6);

        LA_CORE_DEBUG("ECS: InsertAndSetTest passed");
    }

    void _SystemTest() {
        World world;

        auto entity1 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity>(entity1, [](_Position& pos, _Velocity& vel){
            pos = { 1, 8 };
            vel = { 2, 9 };
        });

        auto entity2 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity>(entity2, [](_Position& pos, _Velocity& vel){
            pos = { 1, 8 };
            vel = { 2, 9 };
        });

        auto entity3 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity, _Transform>(entity3, [](_Position& pos, _Velocity& vel, _Transform& trans){
            pos = { 1, 8 };
            vel = { 2, 9 };
            trans = { 1, 2, 3 };
        });

        auto entity4 = world.CreateEntity();
        world.InsertComponent<_Position>(entity4, [](_Position& pos){
            pos = { 1, 8 };
        });

        int count = 0;
        world.System<_Position, _Velocity>([&](_Position& pos, _Velocity& vel){
                count++;
                assert(pos.x == 1 && pos.y == 8);
                assert(vel.x == 2 && vel.y == 9);
            });
        assert(count == 3);

        world.InsertComponent<_Velocity>(entity4, [](_Velocity& vel){
            vel = { 2, 9 };
        });

        count = 0;
        world.System<_Position, _Velocity>([&](_Position& pos, _Velocity& vel){
                count++;
                assert(pos.x == 1 && pos.y == 8);
                assert(vel.x == 2 && vel.y == 9);
            });
        assert(count == 4);

        LA_CORE_DEBUG("ECS: SystemTest passed");
    }

    void _CreateAndDelete() {
        World world;

        for (int i = 0; i < 10; i++) {
            auto entity1 = world.CreateEntity();
            world.InsertComponent<_Position, _Velocity>(entity1, [=](_Position& pos, _Velocity& vel){
                pos = { (float)i, 8 };
                vel = { 2.0f * i, 9 };
            });
        }

        auto my_entity1 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity>(my_entity1, [](_Position& pos, _Velocity& vel){
            pos = { 49, 49 };
            vel = { 2.0f, 9 };
        });

        auto my_entity2 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity>(my_entity2, [](_Position& pos, _Velocity& vel){
            pos = { 50, 50 };
            vel = { 2.0f, 9 };
        });

        for (int i = 0; i < 20; i++) {
            auto entity1 = world.CreateEntity();
            world.InsertComponent<_Position, _Velocity>(entity1, [=](_Position& pos, _Velocity& vel){
                pos = { (float)i, 8 };
                vel = { 2.0f * i, 9 };
            });
        }

        auto entity1_copy = world.GetEntity(my_entity1.GetId());
        assert(entity1_copy.has_value());
        assert(entity1_copy.value().GetId() == my_entity1.GetId());

        world.KillEntity(my_entity1);
        assert(!world.GetEntity(my_entity1.GetId()).has_value());

        LA_CORE_DEBUG("ECS: CreateAndDelete passed");
    }

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
        _InsertAndSetTest();
        _SystemTest();
        _CreateAndDelete();
    }

#endif
}
