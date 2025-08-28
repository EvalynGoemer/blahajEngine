#include "lua.h"
#include "engine.h"
#include "aabb.h"

namespace blahajEngine {

static int lua_addPipeline(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    int pipelineID = lua_tonumber(L, 2);
    const char *vertexShader = lua_tostring(L, 3);
    const char *fragmentShader = lua_tostring(L, 4);

    VulkanRenderer *vulkanRenderer = app->getVulkanRenderer();
    vulkanRenderer->createGraphicsPipeline(pipelineID, vertexShader,
                                           fragmentShader);

    return 0;
}

static int lua_addTexture(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    int textureID = lua_tonumber(L, 2);
    const char *texture = lua_tostring(L, 3);

    VulkanRenderer *vulkanRenderer = app->getVulkanRenderer();
    vulkanRenderer->createTextureImage(textureID, texture);

    return 0;
}

static int lua_addGameObject(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);

    glm::vec3 pos = {lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)};
    glm::vec3 rot = {lua_tonumber(L, 5), lua_tonumber(L, 6), lua_tonumber(L, 7)};
    glm::vec3 scale = glm::vec3(lua_tonumber(L, 8));

    int pipelineID = lua_tointeger(L, 9);
    int textureID = lua_tointeger(L, 10);
    const char *scriptPath = "nil";
    if (lua_isstring(L, 11)) {
        scriptPath = lua_tostring(L, 11);
    }

    int id;
    int nextId = app->getNextId();
    std::vector<int> deletedIds = app->getDeletedIds();
    if (!deletedIds.empty()) {
        id = deletedIds.back();
        deletedIds.pop_back();
    } else {
        id = nextId + 1;
        nextId++;
    }
    app->setNextId(nextId);
    app->setDeletedIds(deletedIds);

    app->addGameObject(
        id, pos, rot, scale, pipelineID, textureID, scriptPath,
        [app](std::shared_ptr<blahajEngine::gameObject> object,
              blahajEngine::UniformBufferObject &ubo) {
            lua_State *L = app->getLuaState();
            VulkanRenderer *vulkanRenderer = app->getVulkanRenderer();

            if (object->scriptFile != "nil") {
                push_shared_ptr_to_lua(L, object);
                lua_setglobal(L, "object");

                push_raw_ptr_to_lua(L, &ubo);
                lua_setglobal(L, "ubo");

                if (luaL_dostring(L, object->script.c_str()) != LUA_OK) {
                    const char *error = lua_tostring(L, -1);
                    lua_pop(L, 1);
                    throw std::runtime_error("BlahajEngine [LUA]: " +
                    std::string(error));
                }

                lua_getglobal(L, "Update");

                if (lua_isfunction(L, -1)) {
                    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
                        const char *error = lua_tostring(L, -1);
                        lua_pop(L, 1);
                        throw std::runtime_error("BlahajEngine [LUA]: " +
                        std::string(error));
                    }
                } else {
                    throw std::runtime_error(
                        "BlahajEngine [LUA]: Could not find function Update() inside " +
                        object->scriptFile);
                }
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, object->pos);
            model = glm::scale(model, object->scale);

            ubo.model = model;
            camera cam = vulkanRenderer->getCamera();
            ubo.view = glm::lookAt(cam.cameraPos, cam.cameraTarget, cam.upVector);

            float zoom = 600.0f;

            VkExtent2D swapChainExtent = vulkanRenderer->getSwapChainExtent();
            float width = swapChainExtent.width / zoom;
            float height = swapChainExtent.height / zoom;
            float left = -width / 2.0f;
            float right = width / 2.0f;
            float bottom = -height / 2.0f;
            float top = height / 2.0f;
            float nearPlane = 0.1f;
            float farPlane = 10.0f;

            cam.aabbMin = {left, bottom};
            cam.aabbMax = {right, top};

            vulkanRenderer->setCamera(cam);

            ubo.proj = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
            ubo.proj[1][1] *= -1;

            app->setLuaState(L);
              });

    std::vector<std::shared_ptr<blahajEngine::gameObject>> gameObjectsStaging =
    app->getGameObjectsStaging();
    auto it =
    std::find_if(gameObjectsStaging.begin(), gameObjectsStaging.end(),
                 [id](const std::shared_ptr<blahajEngine::gameObject> &obj) {
                     return obj->id == id;
                 });

    if (it != gameObjectsStaging.end()) {
        auto foundObject = *it;
        // TODO Mayber change to app->L using getter just incase it breaks
        push_shared_ptr_to_lua(L, foundObject);
    } else {
        throw std::runtime_error(
            "BlahajEngine [LUA]: \"Could not find object with an ID that was just "
            "used to make an object??????????\"");
    }

    return 1;
}

static int lua_deleteGameObject(lua_State *L) {
    auto obj = pop_shared_ptr_from_lua<gameObject>(L, 1);

    obj->markedForDeletion = true;

    return 0;
}

static int lua_moveGameObject(lua_State *L) {
    auto obj = pop_shared_ptr_from_lua<gameObject>(L, 1);

    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);

    obj->pos = {x, y, z};

    return 0;
}

static int lua_camLookAtGameObject(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    auto obj = pop_shared_ptr_from_lua<gameObject>(L, 2);

    VulkanRenderer *vulkanRenderer = app->getVulkanRenderer();
    camera cam = vulkanRenderer->getCamera();

    cam.cameraPos = obj->pos;
    cam.cameraPos.z = 5;

    cam.cameraTarget = obj->pos;

    vulkanRenderer->setCamera(cam);

    return 0;
}

static int lua_getKeyPressed(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    const char *str = lua_tostring(L, 2);

    GLFWwindow *window = app->getWindow();
    lua_pushboolean(L, glfwGetKey(window, str[0]) == GLFW_PRESS);

    return 1;
}

static int lua_getLeftMousePress(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    GLFWwindow *window = app->getWindow();

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float nx = xpos / width;
        float ny = 1.0f - (ypos / height);

        VulkanRenderer *vulkanRenderer = app->getVulkanRenderer();
        camera cam = vulkanRenderer->getCamera();

        float worldX = (cam.aabbMin.x + nx * (cam.aabbMax.x - cam.aabbMin.x)) +
        cam.cameraPos.x;
        float worldY = (cam.aabbMin.y + ny * (cam.aabbMax.y - cam.aabbMin.y)) +
        cam.cameraPos.y;

        lua_pushnumber(L, worldX);
        lua_pushnumber(L, worldY);

        return 2;
    }

    return 0;
}

static int lua_getRightMousePress(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    GLFWwindow *window = app->getWindow();

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float nx = xpos / width;
        float ny = 1.0f - (ypos / height);

        VulkanRenderer *vulkanRenderer = app->getVulkanRenderer();
        camera cam = vulkanRenderer->getCamera();

        float worldX = (cam.aabbMin.x + nx * (cam.aabbMax.x - cam.aabbMin.x)) +
        cam.cameraPos.x;
        float worldY = (cam.aabbMin.y + ny * (cam.aabbMax.y - cam.aabbMin.y)) +
        cam.cameraPos.y;

        lua_pushnumber(L, worldX);
        lua_pushnumber(L, worldY);

        return 2;
    }

    return 0;
}

static int lua_AABB2D_intersectsAll(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    auto obj = pop_shared_ptr_from_lua<gameObject>(L, 2);
    std::vector<std::shared_ptr<blahajEngine::gameObject>> gameObjects =
    app->getGameObjects();

    lua_pushboolean(L, AABB2d::intersectsAll(obj, gameObjects));

    return 1;
}

static int lua_AABB2D_intersectsFirst(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    auto obj = pop_shared_ptr_from_lua<gameObject>(L, 2);
    std::vector<std::shared_ptr<blahajEngine::gameObject>> gameObjects =
    app->getGameObjects();

    std::shared_ptr<gameObject> objB = AABB2d::intersectsFirst(obj, gameObjects);

    if (objB != nullptr) {
        push_shared_ptr_to_lua(L, objB);
        return 1;
    }

    return 0;
}

static int lua_setProgramName(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    auto programName = lua_tostring(L, 2);
    GLFWwindow *window = app->getWindow();

    glfwSetWindowTitle(window, programName);

    return 0;
}

static int lua_setTargetFPS(lua_State *L) {
    auto *app = pop_raw_ptr_from_lua<blahajEngine::engine>(L, 1);
    int fps = lua_tonumber(L, 2);

    app->setTargetFPS(fps);

    return 0;
}

void blahajEngine::lua::bindAllEngineFuncs(lua_State *L) {
    lua_register(L, "addPipeline", lua_addPipeline);
    lua_register(L, "addTexture", lua_addTexture);
    lua_register(L, "addGameObject", lua_addGameObject);
    lua_register(L, "deleteGameObject", lua_deleteGameObject);
    lua_register(L, "moveGameObject", lua_moveGameObject);
    lua_register(L, "camLookAtGameObject", lua_camLookAtGameObject);
    lua_register(L, "getKeyPressed", lua_getKeyPressed);
    lua_register(L, "getLeftMousePress", lua_getLeftMousePress);
    lua_register(L, "getRightMousePress", lua_getRightMousePress);
    lua_register(L, "AABB2D_intersectsAll", lua_AABB2D_intersectsAll);
    lua_register(L, "AABB2D_intersectsFirst", lua_AABB2D_intersectsFirst);
    lua_register(L, "setProgramName", lua_setProgramName);
    lua_register(L, "setTargetFPS", lua_setTargetFPS);
}
}
