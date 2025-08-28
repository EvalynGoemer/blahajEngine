#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>
#include <memory>
#include <map>

#include "engine_structs.h"

namespace blahajEngine {
    class VulkanRenderer{
    public:
        VulkanRenderer();
        ~VulkanRenderer();

        void init(GLFWwindow* window);
        void cleanup(std::vector<std::shared_ptr<blahajEngine::gameObject>>& gameObjects);
        void drawFrame(std::vector<std::shared_ptr<blahajEngine::gameObject>>& gameObjects, std::vector<std::shared_ptr<blahajEngine::gameObject>>& gameObjectsStaging);

        void createGraphicsPipeline(int id, std::string vertShaderPath, std::string fragShaderPath);

        void createVertexBuffer(int id, std::vector<Vertex> vertices);
        void createIndexBuffer(int id, std::vector<uint16_t> indices);
        void createTextureImage(int id, std::string imagePath);


        VkExtent2D getSwapChainExtent() const;
        void setSwapChainExtent(const VkExtent2D& extent);

        const camera& getCamera() const;
        void setCamera(const camera& c);

        const std::map<int, std::vector<Vertex>>& getVertices() const;
        void setVertices(const std::map<int, std::vector<Vertex>>& v);

        const std::map<int, std::vector<uint16_t>>& getIndices() const;
        void setIndices(const std::map<int, std::vector<uint16_t>>& i);

        bool isFramebufferResized() const;
        void setFramebufferResized(bool resized);

        GLFWwindow* getWindow() const;
        void setWindow(GLFWwindow* win);
    private:
        void createDescriptorSets(int id);
    };
}
