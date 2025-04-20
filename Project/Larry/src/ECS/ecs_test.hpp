#pragma once
#include "ECS/ECS.h"
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

    int _testComponentChildDestructed = 0;
    int _testComponentFatherDestructed = 0;
    int _testComponentOutsideDestructed = 0;

    struct _TestComponentChild {
        int temp;

        ~_TestComponentChild() {
            _testComponentChildDestructed++;
            /* LA_CORE_DEBUG("_TestComponentChild destructed"); */
        };
    };

    struct _TestComponentFather {
        int x;
        _TestComponentChild child;

        ~_TestComponentFather() {
            _testComponentFatherDestructed++;
            /* LA_CORE_DEBUG("_TestComponentFather destructed"); */
        };
    };

    struct _TestComponentOutside {
        int x;

        ~_TestComponentOutside () {
            _testComponentOutsideDestructed++;
            /* LA_CORE_DEBUG("_TestComponentOutside destructed"); */
        };
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
            pos = { 1 * 2, 8 };
            vel = { 2, 9 };
        });

        auto entity3 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity, _Transform>(entity3, [](_Position& pos, _Velocity& vel, _Transform& trans){
            pos = { 1 * 3, 8 };
            vel = { 2, 9 };
            trans = { 1, 2, 3 };
        });

        auto entity4 = world.CreateEntity();
        world.InsertComponent<_Position>(entity4, [](_Position& pos){
            pos = { 1 * 4, 8 };
        });

        int count = 0;
        std::set<int> pos_x = {1, 2, 3};
        world.System<_Position, _Velocity>([&](_Position& pos, _Velocity& vel){
                count++;
                auto res = pos_x.find(pos.x);
                assert(res != pos_x.end());
                pos_x.erase(res);
                assert(pos.y == 8);
                assert(vel.x == 2 && vel.y == 9);
            });
        assert(count == 3);
        assert(pos_x.empty());

        world.InsertComponent<_Velocity>(entity4, [](_Velocity& vel){
            vel = { 2, 9 };
        });

        count = 0;
        pos_x = {1, 2, 3, 4};
        world.System<_Position, _Velocity>([&](_Position& pos, _Velocity& vel){
                count++;
                auto res = pos_x.find(pos.x);
                assert(res != pos_x.end());
                pos_x.erase(res);
                assert(pos.y == 8);
                assert(vel.x == 2 && vel.y == 9);
            });
        assert(count == 4);
        assert(pos_x.empty());

        LA_CORE_DEBUG("ECS: SystemTest passed");
    }

    void _CreateAndDeleteTest() {
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

        int my_entity1_index = my_entity1.GetIndex();
        world.KillEntity(my_entity1);
        assert(!world.GetEntity(my_entity1.GetId()).has_value());

        auto my_entity3 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity>(my_entity3, [](_Position& pos, _Velocity& vel){
            pos = { 77, 77 };
            vel = { 2.0f, 9 };
        });

        assert(my_entity3.GetIndex() == my_entity1_index);

        auto pos = world.GetComponent<_Position>(my_entity3);
        assert(pos.has_value());
        assert(pos.value()->x == 77 && pos.value()->y == 77);

        LA_CORE_DEBUG("ECS: CreateAndDelete passed");
    }

    void _DeleteComponentTest() {
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

        world.DeleteComponent<_Position>(entity1);
        assert(!world.GetComponent<_Position>(entity1).has_value());
        assert(world.GetComponent<_Position>(entity2).has_value());

        LA_CORE_DEBUG("ECS: DeleteComponent passed");
    }

    void _DestractorTest() {
        World world;
        
        auto entity = world.CreateEntity();
        world.InsertComponent<_TestComponentFather, _TestComponentOutside>(entity, [](_TestComponentFather& father, _TestComponentOutside& outside){
            father.x = 10;
            father.child.temp = 9;
            outside.x = 9;
        });

        auto father = world.GetComponent<_TestComponentFather>(entity);

        world.SetComponents<_TestComponentFather>(entity, [](_TestComponentFather& father){
            father.x = 10;
            father.child.temp = 9;
        });

        world.System<_TestComponentFather>([&](const _TestComponentFather& father){
            assert(father.x == 10);
            assert(father.child.temp == 9);
        });

        assert(_testComponentFatherDestructed == 0);
        assert(_testComponentChildDestructed == 0);
        assert(_testComponentOutsideDestructed == 0);

        world.DeleteComponent<_TestComponentFather>(entity);

        assert(_testComponentFatherDestructed == 1);
        assert(_testComponentChildDestructed == 1);
        assert(_testComponentOutsideDestructed == 0);

        world.KillEntity(entity);

        assert(_testComponentFatherDestructed == 1);
        assert(_testComponentChildDestructed == 1);
        assert(_testComponentOutsideDestructed == 1);

        auto entity2 = world.CreateEntity();
        world.InsertComponent<_TestComponentFather>(entity2, [](_TestComponentFather& father){
            father.x = 10;
            father.child.temp = 9;
        });
        world.KillEntity(entity2);

        assert(_testComponentFatherDestructed == 2);
        assert(_testComponentChildDestructed == 2);

        {
            _TestComponentFather test;
        }

        assert(_testComponentFatherDestructed == 3);
        assert(_testComponentChildDestructed == 3);

        LA_CORE_DEBUG("ECS: Destractor passed");
    }

    void _SingeltonTest() {
        World world;

        world.CreateSingelton<_TestComponentChild>([](_TestComponentChild& child){
            child.temp = 99;
        });

        std::optional<_TestComponentChild*> sing_opt = world.GetSingelton<_TestComponentChild>();
        assert(sing_opt.has_value());
        _TestComponentChild* sing = sing_opt.value();
        assert(sing->temp == 99);

        world.SetSingelton<_TestComponentChild>([](_TestComponentChild& child){
            child.temp = 100;
        });

        sing_opt = world.GetSingelton<_TestComponentChild>();
        assert(sing_opt.has_value());
        sing = sing_opt.value();
        assert(sing->temp == 100);

        auto entity = world.CreateEntity();
        bool succes = world.InsertComponent<_TestComponentChild>(entity, [](_TestComponentChild& child){
            assert(false);
        });
        assert(!succes);

        auto entity1 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity>(entity1, [](_Position& pos, _Velocity& vel){
            pos = { 1, 8 };
            vel = { 2, 9 };
        });

        auto entity2 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity>(entity2, [](_Position& pos, _Velocity& vel){
            pos = { 1 * 2, 8 };
            vel = { 2, 9 };
        });

        auto entity3 = world.CreateEntity();
        world.InsertComponent<_Position, _Velocity, _Transform>(entity3, [](_Position& pos, _Velocity& vel, _Transform& trans){
            pos = { 1 * 3, 8 };
            vel = { 2, 9 };
            trans = { 1, 2, 3 };
        });

        int count = 0;
        world.System<_TestComponentChild, _Position>([&](_TestComponentChild& child, _Position& pos){
            count++;
            assert(child.temp == 100);
            assert(pos.y == 8);
        });

        assert(count == 3);

        LA_CORE_DEBUG("ECS: Singelton passed");
    }

    void TestECS()
    {
        _InsertAndSetTest();
        _SystemTest();
        _CreateAndDeleteTest();
        _DeleteComponentTest();
        _DestractorTest();
        _SingeltonTest();

        LA_CORE_DEBUG("ECS: Test passed");
    }

#ifdef LARRY_ENABLE_TESTING
#endif
}
