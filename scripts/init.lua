function Init()
    setProgramName(engine, "Blahaj Engine Demo: Top Down 2D Game")
    setTargetFPS(engine, 900000)

    -- addPipeline(engine, pipelineID, vertexShader, fragmentShader)
    addPipeline(engine, 1, "spv/debug.vert.spv", "spv/debug.frag.spv")

    -- addTexture(engine, textureID, texture)
    addTexture(engine, 1, "assets/null.png")
    addTexture(engine, 2, "assets/debug.png")
    addTexture(engine, 3, "assets/debug_alt.png")

    -- addGameObject(engine, posX, posY, posZ, rotX, rotY, rotZ, scale, pipelineID, textureID, scriptPath)
    addGameObject(engine, 0, 0.6, 0, 0, 0, 0, 0.5, 1, 1, nil)
    addGameObject(engine, 0, -0.3, 0, 0, 0, 0, 0.1, 1, 2, "scripts/testObject.lua")
    addGameObject(engine, 0.3, 0, 0, 0, 0, 0, 0.1, 1, 3, nil)
    addGameObject(engine, -0.3, 0, 0, 0, 0, 0, 0.1, 1, 3, nil)

    addGameObject(engine, 0, 0, 0, 0, 0, 0, 0.1, 1, 1, "scripts/player.lua")

    for i = 1, 1 do
        addGameObject(engine, 1, 1, 0, 0, 0, 0, 0.1, 1, 1, nil)
    end
end
