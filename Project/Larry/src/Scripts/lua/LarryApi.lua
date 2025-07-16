local ffi = require("ffi")
local cdef = require("CDef")

-- TODO: make lua scripts being able to load textures

ffi.cdef(cdef)

ComponentNamesToHash = {}
function AddComponentHash(name, component_type_hash_as_ptr)
    -- Cast the incoming void* back to ECS_TypeHashCode (unsigned long)
    local component_type_hash = ffi.cast("ECS_TypeHashCode", component_type_hash_as_ptr)
    ComponentNamesToHash[name] = component_type_hash;
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
    print("Here dsadsa")
    print(config)
    print(config.CreateMipmap)
    print(world)
    print(path)
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
    print(wall_texture)
    quad.texture = wall_texture
end
