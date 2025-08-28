#include "aabb.h"
#include <cfloat>

namespace blahajEngine {
    glm::vec2 AABB2d::getAABBMinFromVertices(const std::vector<Vertex>& vertices) {
        float minX = FLT_MAX;
        float minY = FLT_MAX;

        for (const auto& vertex : vertices) {
            minX = std::min(minX, vertex.pos.x);
            minY = std::min(minY, vertex.pos.y);
        }

        return glm::vec2(minX, minY);
    }

    glm::vec2 AABB2d::getAABBMaxFromVertices(const std::vector<Vertex>& vertices) {
        float maxX = -FLT_MAX;
        float maxY = -FLT_MAX;

        for (const auto& vertex : vertices) {
            maxX = std::max(maxX, vertex.pos.x);
            maxY = std::max(maxY, vertex.pos.y);
        }

        return glm::vec2(maxX, maxY);
    }

    glm::vec2 AABB2d::getAABBMin(const std::shared_ptr<gameObject>& object) {
        glm::vec2 scaledAABB = glm::vec2(object->aabbMax.x * object->scale.x, object->aabbMax.y * object->scale.y);
        return glm::vec2(object->pos.x - scaledAABB.x, object->pos.y - scaledAABB.y);
    }

    glm::vec2 AABB2d::getAABBMax(const std::shared_ptr<gameObject>& object) {
        glm::vec2 scaledAABB = glm::vec2(object->aabbMax.x * object->scale.x, object->aabbMax.y * object->scale.y);
        return glm::vec2(object->pos.x + scaledAABB.x, object->pos.y + scaledAABB.y);
    }

    glm::vec2 AABB2d::getAABBMin(camera object) {
        glm::vec2 scaledAABB = glm::vec2(object.aabbMax.x * object.cameraScale.x, object.aabbMax.y * object.cameraScale.y);
        return glm::vec2(object.cameraPos.x - scaledAABB.x, object.cameraPos.y - scaledAABB.y);
    }

    glm::vec2 AABB2d::getAABBMax(camera object) {
        glm::vec2 scaledAABB = glm::vec2(object.aabbMax.x * object.cameraScale.x, object.aabbMax.y * object.cameraScale.y);
        return glm::vec2(object.cameraPos.x + scaledAABB.x, object.cameraPos.y + scaledAABB.y);
    }

    bool AABB2d::intersects(std::shared_ptr<gameObject> objectA, std::shared_ptr<gameObject> objectB) {
        glm::vec2 aabbMinA = getAABBMin(objectA);
        glm::vec2 aabbMaxA = getAABBMax(objectA);
        glm::vec2 aabbMinB = getAABBMin(objectB);
        glm::vec2 aabbMaxB = getAABBMax(objectB);

        return !(aabbMaxA.x < aabbMinB.x ||
        aabbMinA.x > aabbMaxB.x ||
        aabbMaxA.y < aabbMinB.y ||
        aabbMinA.y > aabbMaxB.y);
    }

    bool AABB2d::intersects(std::shared_ptr<gameObject> objectA, camera objectB) {
        glm::vec2 aabbMinA = getAABBMin(objectA);
        glm::vec2 aabbMaxA = getAABBMax(objectA);
        glm::vec2 aabbMinB = getAABBMin(objectB);
        glm::vec2 aabbMaxB = getAABBMax(objectB);

        return !(aabbMaxA.x < aabbMinB.x ||
        aabbMinA.x > aabbMaxB.x ||
        aabbMaxA.y < aabbMinB.y ||
        aabbMinA.y > aabbMaxB.y);
    }

    bool AABB2d::contains(std::shared_ptr<gameObject> objectA, const glm::vec2& point) {
        glm::vec2 aabbMinA = getAABBMin(objectA);
        glm::vec2 aabbMaxA = getAABBMax(objectA);

        return (point.x >= aabbMinA.x && point.x <= aabbMaxA.x) &&
        (point.y >= aabbMinA.y && point.y <= aabbMaxA.y);
    }

    bool AABB2d::intersectsAll(std::shared_ptr<gameObject> objectA, std::vector<std::shared_ptr<gameObject>>& objects) {
        for (auto& objectB : objects) {
            if (objectA == objectB) continue;
            if (intersects(objectA, objectB)) return true;
        }
        return false;
    }

    std::shared_ptr<gameObject> AABB2d::intersectsFirst(std::shared_ptr<gameObject> objectA, std::vector<std::shared_ptr<gameObject>>& objects) {
        for (auto& objectB : objects) {
            if (objectA == objectB) continue;
            if (intersects(objectA, objectB)) return objectB;
        }
        return nullptr;
    }
}
