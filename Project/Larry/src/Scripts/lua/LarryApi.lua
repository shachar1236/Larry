local ffi = require("ffi")
local cdef = require("CDef")
local inspect = require("inspect")

-- TODO: make lua scripts being able to load textures

ffi.cdef(cdef)

ComponentNamesToHash = {}
ComponentHashToNames = {}
function AddComponentHash(name, component_type_hash_as_ptr)
    -- Cast the incoming void* back to ECS_TypeHashCode (unsigned long)
    local component_type_hash = ffi.cast("ECS_TypeHashCode", component_type_hash_as_ptr)
    ComponentNamesToHash[name] = component_type_hash;
    ComponentHashToNames[component_type_hash] = name
end

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
    print("In SystemCallback")
    local entity = ffi.cast("int64_t", entity_as_ptr);
    local stop = ffi.cast("bool*", stop_as_voidptr)
    local component_table = {}
    for i, comp_name in ipairs(currentSystemComponents) do
        print("Extracting component ", comp_name)
        local any_type = ffi.C.ECS_PopFromAnyQueue(components_queue)
        local comp = ffi.cast("struct " .. comp_name .. "*", any_type.value)
        table.insert(component_table, comp)
    end
    print(inspect(component_table))
    print("Calling currentSystemCallback")
    currentSystemCallback(entity, stop, unpack(component_table))
end

function System(world, components, callback)
    print("Hiiiiiiii")
    local type_quaue = ffi.C.ECS_InitTypeQueue(world);
    local any_queue = ffi.C.ECS_InitAnyQueue(world);
    print("Hiiiiiiii2")
    
    for i, comp in ipairs(components) do
        print(comp)
        local hash = ComponentNamesToHash[comp]
        if not hash then
            print("Didnt found hash")
            goto done
        end
        ffi.C.ECS_PushToTypeQueue(type_quaue, hash)
        print("Hiiiiiiii3")
    end

    currentSystemComponents = components
    currentSystemCallback = callback

    print(ffi.C.LuaECSSystem)
    print("Before calling ecs system")
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
end
