
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
#include <Components/Background.h>
#include <Components/Button.h>
#include <Components/Camera.h>
#include <Components/Projection.h>
#include <Components/Quad.h>
#include <Components/Relationship.h>
#include <Components/Transform.h>

using namespace Larry::Math;
using namespace Larry;
using namespace Larry::ECS;


extern "C" {
    void _Camera_SetPos(void* __obj, Vec3 pos)
    {
        Camera* __casted_obj = static_cast<Camera*>(__obj);
        return __casted_obj->SetPos(pos);
    }

    Background _create_Background()
    {
        return Background();
    }

    Button _create_Button()
    {
        return Button();
    }

    Quad _create_Quad()
    {
        return Quad();
    }

    Transform _create_Transform()
    {
        return Transform();
    }

    void _Transform_Reset(Transform* __obj, int test)
    {
        return __obj->Reset(test);
    }

    void _destruct_Transform(Transform* __obj)
    {
        __obj->~Transform();
    }

}