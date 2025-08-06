local ffi = require("ffi")
local cdef = require("CDef")
local inspect = require("lib.inspect")
local dbg = require("lib.debugger")

ffi.cdef(cdef)

ComponentNamesToHash = {}
-- function AddComponentHash(name, component_type_hash_as_ptr)
    -- Cast the incoming void* back to ECS_TypeHashCode (unsigned long)
    -- local component_type_hash = ffi.cast("ECS_TypeHashCode", component_type_hash_as_ptr)
    -- ComponentNamesToHash[name] = component_type_hash;
-- end

function GetComponent(world, entity, componentName)
    -- Cast the incoming void* back to ECS_TypeHashCode (unsigned long)
    -- local component_type_hash = ffi.cast("ECS_TypeHashCode", component_type_hash_as_ptr)

    if ComponentNamesToHash[componentName] then
        local res = ffi.C.ECS_GetComponent(world, entity, ComponentNamesToHash[componentName])
        local comp = ffi.cast("struct " .. componentName .. "*", res.value)
        return comp
        -- print("Lua: ECS_GetComponent returned. Value:", res.value, "Type:", res.type)
    end
end

currentSystemCallback = nil
currentSystemComponents = {}
function SystemCallback(entity_as_ptr, components_queue, stop_as_voidptr)
    local entity = ffi.cast("int64_t", entity_as_ptr);
    local stop = ffi.cast("bool*", stop_as_voidptr)
    local component_table = {}
    for i, comp_name in ipairs(currentSystemComponents) do
        local any_type = ffi.C.ECS_PopFromAnyQueue(components_queue)
        local comp = ffi.cast("struct " .. comp_name .. "*", any_type.value)
        table.insert(component_table, comp)
    end
    currentSystemCallback(entity, stop, unpack(component_table))
end

function System(world, components, callback)
    local type_quaue = ffi.C.ECS_InitTypeQueue(world);
    local any_queue = ffi.C.ECS_InitAnyQueue(world);
    
    for i, comp in ipairs(components) do
        local hash = ComponentNamesToHash[comp]
        if not hash then
            goto done
        end
        ffi.C.ECS_PushToTypeQueue(type_quaue, hash)
    end

    currentSystemComponents = components
    currentSystemCallback = callback

    print(ffi.C.LuaECSSystem)
    ffi.C.ECS_System(world, type_quaue, any_queue, ffi.C.LuaECSSystem)
    
    ::done::
    ffi.C.ECS_DoneWithTypeQueue(world, type_quaue)
    ffi.C.ECS_DoneWithAnyQueue(world, any_queue)
end

TextureWrappingOptions = {
    REPEAT = ffi.C.REPEAT,
    MIRRORED_REPEAT  = ffi.C.MIRRORED_REPEAT,
    CLAMP_TO_EDGE = ffi.C.CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = ffi.C.CLAMP_TO_BORDER
}

MipmapFilterOptions = {
    NEAREST_MIPMAP_NEAREST = ffi.C.NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = ffi.C.LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = ffi.C.NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = ffi.C.LINEAR_MIPMAP_LINEAR
}

TextureFilterOptions = {
    NEAREST = ffi.C.NEAREST,
    LINEAR = ffi.C.LINEAR
}

function CreateTextureConfig(params)
    return ffi.new("TextureConfig", {
        params.CreateMipmap or true,
        params.TextureWrappingS or TextureWrappingOptions.REPEAT,
        params.TextureWrappingT or TextureWrappingOptions.REPEAT,
        params.TextureFilterMin or TextureFilterOptions.LINEAR,
        params.TextureFilterMag or TextureFilterOptions.LINEAR,
        params.MipmapFilterMin or MipmapFilterOptions.LINEAR_MIPMAP_LINEAR,
    })
end

-- TextureLoader --
function LoadTexture(world, path, config)
    local t = ffi.C.LuaLoadTexture(world, path, config)
    return t
end

-- Input --
function KeyPressed(key)
    return ffi.C.LuaKeyPressed(key)
end

function MouseX()
    return ffi.C.LuaGetMouseX()
end

function MouseY()
    return ffi.C.LuaGetMouseY()
end

function Mouse()
    return ffi.new("Vec2", { MouseX(), MouseY() })
end
-- scripts --
Script = {}

function Script:New(o)
    o.parent = self
    return o
end

function Script:OnCreate(world, entity)
    print("Script has been created")
end

function Script:OnUpdate(deltaTime)
    print("Script has been updated")
end

function Script:OnDelete()
    print("Script has been deleted")
end

function CallScriptOnCreate(script, world, entity_as_ptr)
    local entity = ffi.cast("int64_t", entity_as_ptr);
    script:OnCreate(world, entity)
end

function CallScriptOnUpdate(script, deltaTime)
    -- print(script.name)
    script:OnUpdate(deltaTime)
end

function CallScriptOnDelete(script)
    script:OnDelete()
end
-- test --

function Test(world, entity_as_ptr)
    local entity = ffi.cast("int64_t", entity_as_ptr);

    local asTr = GetComponent(world, entity, "Transform")
    print(asTr.translation.x .. " " .. asTr.translation.y .. " " .. asTr.translation.z)
    print(asTr.scale.x .. " " .. asTr.scale.y .. " " .. asTr.scale.z)
    print(asTr.rotation_axis.x .. " " .. asTr.rotation_axis.y .. " " .. asTr.rotation_axis.z)
    print(asTr.rotation_size)

    local wall_texture = LoadTexture(world, "media/textures/wall.jpg", CreateTextureConfig({}))
    local quad = GetComponent(world, entity, "Quad")
    print(quad.dimentions.x, quad.dimentions.x)
    quad.texture = wall_texture

    System(world, { "Transform", "Quad" }, function (entity_, stop, transform, quad_ref)
        print("inside system function")
        transform.rotation_size = 1
        quad_ref.color.y = 0;
    end)

    print("Type of script: ", type(Script:New({})))
end
