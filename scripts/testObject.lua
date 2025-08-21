function Update()
--     if Ticks == nil then
--         Ticks = 0;
--     end
--
--     Ticks = Ticks + 1
--     print(Ticks)

--     if getKeyPressed(engine, "O") then
        local obj2 =  AABB2D_intersectsFirst(engine, object)
        if obj2 ~= nil then
            deleteGameObject(obj2)
            obj2 = nil
        end
--     end

--     if Ticks > 20 then
        deleteGameObject(object)
--     end
end
