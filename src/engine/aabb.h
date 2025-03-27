#pragma once

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
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

#include "engine_structs.h"

namespace blahajEngine {
    class AABB2d {
    public:
        static glm::vec2 getAABBMinFromVertices(const std::vector<Vertex>& vertices) {
            float minX = FLT_MAX;
            float minY = FLT_MAX;

            for (const auto& vertex : vertices) {
                minX = std::min(minX, vertex.pos.x);
                minY = std::min(minY, vertex.pos.y);
            }

            return glm::vec2(minX, minY);
        }

        static glm::vec2 getAABBMaxFromVertices(const std::vector<Vertex>& vertices) {
            float maxX = -FLT_MAX;
            float maxY = -FLT_MAX;

            for (const auto& vertex : vertices) {
                maxX = std::max(maxX, vertex.pos.x);
                maxY = std::max(maxY, vertex.pos.y);
            }

            return glm::vec2(maxX, maxY);
        }

        static glm::vec2 getAABBMin(const std::shared_ptr<gameObject>& object) {
            glm::vec2 scaledAABB = glm::vec2(object->aabbMax.x * object->scale.x, object->aabbMax.y * object->scale.y);
            glm::vec2 aabbMin = glm::vec2(object->pos.x - scaledAABB.x, object->pos.y - scaledAABB.y);
            return aabbMin;
        }

        static glm::vec2 getAABBMax(const std::shared_ptr<gameObject>& object) {
            glm::vec2 scaledAABB = glm::vec2(object->aabbMax.x * object->scale.x, object->aabbMax.y * object->scale.y);
            glm::vec2 aabbMax = glm::vec2(object->pos.x + scaledAABB.x, object->pos.y + scaledAABB.y);
            return aabbMax;
        }

        static bool intersects(std::shared_ptr<gameObject> objectA, std::shared_ptr<gameObject> objectB) {
            glm::vec2 aabbMinA = getAABBMin(objectA);
            glm::vec2 aabbMaxA = getAABBMax(objectA);
            glm::vec2 aabbMinB = getAABBMin(objectB);
            glm::vec2 aabbMaxB = getAABBMax(objectB);

            return !(aabbMaxA.x < aabbMinB.x ||
                    aabbMinA.x > aabbMaxB.x ||
                    aabbMaxA.y < aabbMinB.y ||
                    aabbMinA.y > aabbMaxB.y);
        }

        static bool contains(std::shared_ptr<gameObject> objectA, const glm::vec2& point) {
            glm::vec2 aabbMinA = getAABBMin(objectA);
            glm::vec2 aabbMaxA = getAABBMax(objectA);

            return (point.x >= aabbMinA.x && point.x <= aabbMaxA.x) &&
            (point.y >= aabbMinA.y && point.y <= aabbMaxA.y);
        }

        static bool intersectsAll(std::shared_ptr<gameObject> objectA, std::vector<std::shared_ptr<gameObject>>& objects) {
            for (auto& objectB : objects) {
                if (objectA == objectB) {
                    continue;
                }

                if (intersects(objectA, objectB)) {
                    return true;
                }
            }
            return false;
        }
    };
}
