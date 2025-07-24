keys = require("LarryKeyCodes")
TestLuaScript = {}

function TestLuaScript:New(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function TestLuaScript:OnCreate(world, entity)
    print("Test Script has been created")
    self.world = world
    self.entity = entity
    self.vel = 400
end

function TestLuaScript:OnUpdate(deltaTime)
    local direction = 0
    if KeyPressed(keys["KEY_W"]) then
        direction = direction + self.vel
    elseif KeyPressed(keys["KEY_S"]) then
        direction = direction - self.vel
    end
    local transform = GetComponent(self.world, self.entity, "Transform")
    transform.translation.y = transform.translation.y + direction * deltaTime
end

function TestLuaScript:OnDelete()
    print("Test Script has been deleted")
end

return TestLuaScript:New()
