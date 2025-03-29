function Init()
    setProgramName(engine, "Blahaj Engine Demo: Top Down 2D Game")
    setTargetFPS(engine, 90)

    addGameObject(engine, 0, 0.6, 0, 0, 0, 0, 0.5, "spv/debug.vert.spv", "spv/debug.frag.spv", "assets/null.png", "scripts/static.lua")
    addGameObject(engine, 0, -0.3, 0, 0, 0, 0, 0.1, "spv/debug.vert.spv", "spv/debug.frag.spv", "assets/debug.png", "scripts/static.lua")
    addGameObject(engine, 0.3, 0, 0, 0, 0, 0, 0.1, "spv/debug.vert.spv", "spv/debug.frag.spv", "assets/debug_alt.png", "scripts/static.lua")
    addGameObject(engine, -0.3, 0, 0, 0, 0, 0, 0.1, "spv/debug.vert.spv", "spv/debug.frag.spv", "assets/debug_alt.png", "scripts/static.lua")

    addGameObject(engine, 0, 0, 0, 0, 0, 0, 0.1, "spv/debug.vert.spv", "spv/debug.frag.spv", "assets/null.png", "scripts/player.lua")
    addGameObject(engine, 1, 1, 0, 0, 0, 0, 0.1, "spv/debug.vert.spv", "spv/debug.frag.spv", "assets/null.png", "scripts/testObject.lua")
end
