function Init()
    setProgramName(engine, "Blahaj Engine Demo: Top Down 2D Game")
    setTargetFPS(engine, 90)

    -- addPipeline(engine, pipelineID, vertexShader, fragmentShaderD)
    addPipeline(engine, 1, "spv/debug.vert.spv", "spv/debug.frag.spv")

    -- addGameObject(engine, posX, posY, posZ, rotX, rotY, rotZ, scale, pipelineID, imagePath, scriptPath)
    addGameObject(engine, 0, 0.6, 0, 0, 0, 0, 0.5, 1, "assets/null.png", nil)
    addGameObject(engine, 0, -0.3, 0, 0, 0, 0, 0.1, 1, "assets/debug.png", nil)
    addGameObject(engine, 0.3, 0, 0, 0, 0, 0, 0.1, 1, "assets/debug_alt.png", nil)
    addGameObject(engine, -0.3, 0, 0, 0, 0, 0, 0.1, 1, "assets/debug_alt.png", nil)

    addGameObject(engine, 0, 0, 0, 0, 0, 0, 0.1, 1, "assets/null.png", "scripts/player.lua")
    addGameObject(engine, 1, 1, 0, 0, 0, 0, 0.1, 1, "assets/null.png", "scripts/testObject.lua")
end
