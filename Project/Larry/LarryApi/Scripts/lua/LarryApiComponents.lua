local ffi = require("ffi")
local custom_cdef = require("CDef")
local cdef = custom_cdef .. [[
typedef struct {
    Vec4 color;
    void* texture;
} Background;

typedef struct {
    Vec2 dimentions;
    Vec4 color;
} Button;

typedef struct {
    Vec3 dimentions;
    Vec4 color;
    void* texture;
} Quad;

typedef struct {
    Vec3 translation;
    Vec3 scale;
    Vec3 rotation_axis;
    float rotation_size;
    Vec3 _realTranslation;
    int _realTranslationTimestemp;
} Transform;

void _Transform_Reset(Transform* __obj, int test);

]]

ffi.cdef(cdef)

local _obj_Transform
local _obj_Transform_mt = {
    __index = {
        Reset = function(obj, test) return ffi.C._Transform_Reset(obj, test) end,
    }
}

_obj_Transform = ffi.metatype("Transform*", _obj_Transform_mt)

