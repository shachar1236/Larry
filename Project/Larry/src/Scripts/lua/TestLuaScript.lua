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
end

function TestLuaScript:OnUpdate(deltaTime)
    print("Test Script has been updated")
end

function TestLuaScript:OnDelete()
    print("Test Script has been deleted")
end

return TestLuaScript:New()
