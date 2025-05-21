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
    if getKeyPressed(engine, "P") then
        deleteGameObject(object)
    end

    if NotFirstRun == nil then
        X = 0
        Y = 0

        NotFirstRun = true
    end

    local speed = 1
    local movementStep = 0.0005
    local velocityX = 0
    local velocityY = 0

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

    velocityX, velocityY = normalizeVelocity(velocityX, velocityY)

    X = X + velocityX * speed * delta_time
    moveGameObject(object, X, Y, 2)

    if AABB2D_intersectsAll(engine, object) then
        while AABB2D_intersectsAll(engine, object) do
            X = X - movementStep * sign(velocityX)
            moveGameObject(object, X, Y, 2)
        end
    end

    Y = Y + velocityY * speed * delta_time
    moveGameObject(object, X, Y, 2)

    if AABB2D_intersectsAll(engine, object) then
        while AABB2D_intersectsAll(engine, object) do
            Y = Y - movementStep * sign(velocityY)
            moveGameObject(object, X, Y, 2)
        end
    end

    camLookAtGameObject(engine, object)
end
