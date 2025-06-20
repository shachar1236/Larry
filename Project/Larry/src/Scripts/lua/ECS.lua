local ffi = require("ffi")
local cdef = require("CDef")

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

function Test(world, entity_as_ptr)
    local entity = ffi.cast("int64_t", entity_as_ptr);

    local asTr = GetComponent(world, entity, "Transform")
    print(asTr.translation.x .. " " .. asTr.translation.y .. " " .. asTr.translation.z)
    print(asTr.scale.x .. " " .. asTr.scale.y .. " " .. asTr.scale.z)
    print(asTr.rotation_axis.x .. " " .. asTr.rotation_axis.y .. " " .. asTr.rotation_axis.z)
    print(asTr.rotation_size)
end
