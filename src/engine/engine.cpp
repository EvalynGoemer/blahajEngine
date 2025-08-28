#include "engine.h"

#include "VulkanRenderer.h"

#include "aabb.h"
#include "engine_structs.h"
#include "lua.h"

#include <GLFW/glfw3.h>

#include <lua.hpp>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

namespace blahajEngine {
    float target_fps = 120.0f;

    lua_State *L;
    std::vector<std::shared_ptr<blahajEngine::gameObject>> gameObjects;
    std::vector<std::shared_ptr<blahajEngine::gameObject>> gameObjectsStaging;

    int nextId = 0;
    std::vector<int> deletedIds;

    GLFWwindow *window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    VulkanRenderer *vulkanRenderer;

    void blahajEngine::engine::run() {
        initWindow();

        vulkanRenderer = new VulkanRenderer();
        vulkanRenderer->init(window);

        initEngine();
        mainLoop();
        cleanup();
    }

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto app = reinterpret_cast<blahajEngine::engine *>(glfwGetWindowUserPointer(window));

        VulkanRenderer* vulkanRenderer = app->getVulkanRenderer();
        vulkanRenderer->setFramebufferResized(true);
    }

    void blahajEngine::engine::initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "blahajEngine Game", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR};

        void blahajEngine::engine::initEngine() {
            std::vector<blahajEngine::Vertex> vertices_quad = {
                {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

                std::vector<uint16_t> indices_quad = {0, 1, 2, 2, 3, 0};

                std::map<int, std::vector<Vertex>> vertices = vulkanRenderer->getVertices();
                vertices[-1] = vertices_quad;
                vulkanRenderer->setVertices(vertices);

                std::map<int, std::vector<uint16_t>> indices = vulkanRenderer->getIndices();
                indices[-1] = indices_quad;
                vulkanRenderer->setIndices(indices);

                vulkanRenderer->createVertexBuffer(-1, vertices_quad);
                vulkanRenderer->createIndexBuffer(-1, indices_quad);

                L = luaL_newstate();
                luaL_openlibs(L);

                push_raw_ptr_to_lua(L, this);
                lua_setglobal(L, "engine");

                lua::bindAllEngineFuncs(L);

                if (luaL_dofile(L, "scripts/init.lua") != LUA_OK) {
                    const char *error = lua_tostring(L, -1);
                    lua_pop(L, 1);
                    throw std::runtime_error("BlahajEngine [LUA]: " + std::string(error));
                }

                lua_getglobal(L, "Init");

                if (lua_isfunction(L, -1)) {
                    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
                        const char *error = lua_tostring(L, -1);
                        lua_pop(L, 1);
                        throw std::runtime_error("BlahajEngine [LUA]: " + std::string(error));
                    }
                } else {
                    throw std::runtime_error("BlahajEngine [LUA]: Could not find function "
                    "Init() inside scripts/init.lua");
                }
        }

        void blahajEngine::engine::mainLoop() {
            auto last_time = std::chrono::high_resolution_clock::now();
            float frame_duration = 1.0f / target_fps;

            while (!glfwWindowShouldClose(window)) {
                auto current_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> delta_time = current_time - last_time;

                last_time = current_time;
                lua_pushnumber(L, delta_time.count());
                lua_setglobal(L, "delta_time");

                if (delta_time.count() < frame_duration) {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(1));
                    continue;
                }

                glfwPollEvents();

                vulkanRenderer->drawFrame(gameObjects, gameObjectsStaging);

                gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),[this](const std::shared_ptr<gameObject> &obj) {
                    if (obj->markedForDeletion) {
                        obj->markedForDeletion = false;
                        deleteGameObject(obj);
                        return true;
                    }
                    return false;
                }),
                gameObjects.end());
            }
        }

        void blahajEngine::engine::cleanup() {
            vulkanRenderer->cleanup(gameObjects);

            glfwDestroyWindow(window);

            glfwTerminate();

            lua_pushnil(L);
            lua_setglobal(L, "engine");

            lua_pushnil(L);
            lua_setglobal(L, "object");

            lua_getglobal(L, "collectgarbage");
            lua_pushstring(L, "collect");
            if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
                std::runtime_error("blahajEngine [LUA]: Error running collectgarbage: " +
                std::string(lua_tostring(L, -1)));
            }

            lua_close(L);
        }

        void blahajEngine::engine::addGameObject( int objectType, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, int pipelineID, int textureID, std::string scriptFile, std::function<void(std::shared_ptr<blahajEngine::gameObject> &object, UniformBufferObject &ubo)> updateFunction) {
            auto object = std::make_shared<blahajEngine::gameObject>(objectType, pos, rot, scale, -1);

            object->scriptFile = scriptFile;
            object->pipelineID = pipelineID;

            if (scriptFile != "nil") {
                std::ifstream file(scriptFile);
                if (!file) {
                    throw std::runtime_error(
                        "BlahajEngine [LUA]: Could not find script file " +
                        object->scriptFile);
                }

                std::stringstream buffer;
                buffer << file.rdbuf();

                object->script = buffer.str();

                file.close();
            }

            object->setUpdateFunction(updateFunction);

            object->texture_id = textureID;

            std::map<int, std::vector<Vertex>> vertices = vulkanRenderer->getVertices();
            object->aabbMin = AABB2d::getAABBMinFromVertices(vertices[object->model_id]);
            object->aabbMax = AABB2d::getAABBMaxFromVertices(vertices[object->model_id]);

            gameObjectsStaging.push_back(object);
          }

          void blahajEngine::engine::deleteGameObject(std::shared_ptr<gameObject> gameObjectPtr) {
              deletedIds.push_back(gameObjectPtr->id);
          }

          float blahajEngine::engine::getTargetFPS() const { return target_fps; }
          void blahajEngine::engine::setTargetFPS(float fps) { target_fps = fps; }

          lua_State* blahajEngine::engine::getLuaState() const { return L; }
          void blahajEngine::engine::setLuaState(lua_State* state) { L = state; }

          const std::vector<std::shared_ptr<blahajEngine::gameObject>>& blahajEngine::engine::getGameObjects() const { return gameObjects; }
          void blahajEngine::engine::setGameObjects(const std::vector<std::shared_ptr<blahajEngine::gameObject>>& objs) { gameObjects = objs; }

          const std::vector<std::shared_ptr<blahajEngine::gameObject>>& blahajEngine::engine::getGameObjectsStaging() const { return gameObjectsStaging; }
          void blahajEngine::engine::setGameObjectsStaging(const std::vector<std::shared_ptr<blahajEngine::gameObject>>& objs) { gameObjectsStaging = objs; }

          int blahajEngine::engine::getNextId() const { return nextId; }
          void blahajEngine::engine::setNextId(int id) { nextId = id; }

          const std::vector<int>& blahajEngine::engine::getDeletedIds() const { return deletedIds; }
          void blahajEngine::engine::setDeletedIds(const std::vector<int>& ids) { deletedIds = ids; }

          GLFWwindow* blahajEngine::engine::getWindow() const { return window; }
          void blahajEngine::engine::setWindow(GLFWwindow* win) { window = win; }

          uint32_t blahajEngine::engine::getWidth() const { return WIDTH; }
          uint32_t blahajEngine::engine::getHeight() const { return HEIGHT; }

          VulkanRenderer* blahajEngine::engine::getVulkanRenderer() const { return vulkanRenderer; }
          void blahajEngine::engine::setVulkanRenderer(VulkanRenderer* renderer) { vulkanRenderer = renderer; }
}
