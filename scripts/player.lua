local function normalizeVelocity(velocityX, velocityY)
    local magnitude = math.sqrt(velocityX^2 + velocityY^2)

    if magnitude > 0 then
        velocityX = velocityX / magnitude
        velocityY = velocityY / magnitude
    end

    return velocityX, velocityY
end

local function sign(value)
    if value > 0 then
        return 1
    elseif value < 0 then
        return -1
    else
        return 0
    end
end

function Update()
    if NotFirstRun == nil then
        X = 0
        Y = 0
        NotFirstRun = true
    end

    local speed = 1
    local movementStep = 0.0005
    local velocityX = 0
    local velocityY = 0

    local x, y = getLeftMousePress(engine)
    if x and y ~= nil then
        addGameObject(engine, x, y, -1, 0, 0, 0, 0.1, 1, 5, nil)
        x = nil
        y = nil
    end

    local x, y = getRightMousePress(engine)
    if x and y ~= nil then
        addGameObject(engine, x, y, -1, 0, 0, 0, 0.1, 1, 4, "scripts/testObject.lua")
        x = nil
        y = nil
    end

    if getKeyPressed(engine, "W") then
        velocityY = velocityY + 1
    end
    if getKeyPressed(engine, "A") then
        velocityX = velocityX - 1
    end
    if getKeyPressed(engine, "S") then
        velocityY = velocityY - 1
    end
    if getKeyPressed(engine, "D") then
        velocityX = velocityX + 1
    end

    if getKeyPressed(engine, "K") and OBJMade == nil then
        OBJ = addGameObject(engine, 1, 1, 0, 0, 0, 0, 0.1, 1, 1, nil)
        OBJMade = true
    end
    if getKeyPressed(engine, "L") and OBJ ~= nil then
        deleteGameObject(OBJ)
        OBJMade = nil
    end

    velocityX, velocityY = normalizeVelocity(velocityX, velocityY)

    local maxIterations = 100
    X = X + velocityX * speed * delta_time
    moveGameObject(object, X, Y, 2)

    if AABB2D_intersectsAll(engine, object) then
        local iterations = 0
        while AABB2D_intersectsAll(engine, object) do
            X = X - movementStep * sign(velocityX)
            moveGameObject(object, X, Y, 2)
            iterations = iterations + 1
            if iterations >= maxIterations then
                X, Y = 0, 0
                moveGameObject(object, X, Y, 2)
                break
            end
        end
    end

    Y = Y + velocityY * speed * delta_time
    moveGameObject(object, X, Y, 2)

    if AABB2D_intersectsAll(engine, object) then
        local iterations = 0
        while AABB2D_intersectsAll(engine, object) do
            Y = Y - movementStep * sign(velocityY)
            moveGameObject(object, X, Y, 2)
            iterations = iterations + 1
            if iterations >= maxIterations then
                X, Y = 0, 0
                moveGameObject(object, X, Y, 2)
                break
            end
        end
    end

    camLookAtGameObject(engine, object)
end
