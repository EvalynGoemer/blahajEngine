#pragma once

#include "lua.hpp"
#include "VulkanRenderer.h"

namespace blahajEngine {
    class engine {
    public:
        void run();

        void addGameObject(int objectType, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, int pipelineID, int textureID, std::string scriptFile, std::function<void(std::shared_ptr<blahajEngine::gameObject> &object, UniformBufferObject &ubo)> updateFunction);
        void deleteGameObject(std::shared_ptr<gameObject> gameObjectPtr);

        float getTargetFPS() const;
        void setTargetFPS(float fps);

        lua_State* getLuaState() const;
        void setLuaState(lua_State* state);

        const std::vector<std::shared_ptr<blahajEngine::gameObject>>& getGameObjects() const;
        void setGameObjects(const std::vector<std::shared_ptr<blahajEngine::gameObject>>& objs);

        const std::vector<std::shared_ptr<blahajEngine::gameObject>>& getGameObjectsStaging() const;
        void setGameObjectsStaging(const std::vector<std::shared_ptr<blahajEngine::gameObject>>& objs);

        int getNextId() const;
        void setNextId(int id);

        const std::vector<int>& getDeletedIds() const;
        void setDeletedIds(const std::vector<int>& ids);

        GLFWwindow* getWindow() const;
        void setWindow(GLFWwindow* win);

        uint32_t getWidth() const;
        uint32_t getHeight() const;

        VulkanRenderer* getVulkanRenderer() const;
        void setVulkanRenderer(VulkanRenderer* renderer);
    private:
        void initWindow();
        void initEngine();
        void mainLoop();
        void cleanup();
    };
}
