#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <vector>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <chrono>

namespace blahajEngine {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };

    struct alignas(16) AlignedInt {
        int i;
    };

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    struct ssboStruct{
        UniformBufferObject UBOs[200000];
    };

    struct pushConstantLayout {
        int id;
        int texture_id;
    };

    enum objectTypes {
        OBJ_TYPE_DEBUG = -1,
        OBJ_TYPE_BACKGROUND = 1
    };

    struct camera {
        glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 1.3f);
        glm::vec3 cameraScale = glm::vec3(1);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector    = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec2 aabbMin;
        glm::vec2 aabbMax;
    };

    struct gameObject {
        int id;
        int texture_id;

        glm::vec3 pos;
        glm::vec3 rot;
        glm::vec3 scale;

        glm::vec2 aabbMin;
        glm::vec2 aabbMax;

        std::string scriptFile;
        std::string script;

        bool markedForDeletion = false;
        bool deleted = false;

        int model_id = -1;

        int pipelineID;

        std::function<void(std::shared_ptr<blahajEngine::gameObject>& object, UniformBufferObject& ubo)> updateFunction;

        void setUpdateFunction(std::function<void(std::shared_ptr<blahajEngine::gameObject>& object, UniformBufferObject& ubo)> func) {
            updateFunction = func;
        }

        void runUpdateFunction(std::shared_ptr<blahajEngine::gameObject>& object, UniformBufferObject& ubo) {
            if (updateFunction) {
                updateFunction(object, ubo);
            }
        }

        gameObject(int id, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, int model_id) {
            this->id = id;
            this->pos = pos;
            this->rot = rot;
            this->scale = scale;
            this->model_id = model_id;
        }

        #ifndef NDEBUG
        ~gameObject() {
            std::cout << "GameObject Running Script: " << scriptFile << " Destroyed & Not Leaked"<< std::endl;
        }
        #endif
    };
}
