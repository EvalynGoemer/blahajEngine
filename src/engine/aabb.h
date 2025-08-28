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
        static glm::vec2 getAABBMinFromVertices(const std::vector<Vertex>& vertices);
        static glm::vec2 getAABBMaxFromVertices(const std::vector<Vertex>& vertices);

        static glm::vec2 getAABBMin(const std::shared_ptr<gameObject>& object);
        static glm::vec2 getAABBMax(const std::shared_ptr<gameObject>& object);

        static glm::vec2 getAABBMin(camera object);
        static glm::vec2 getAABBMax(camera object);

        static bool intersects(std::shared_ptr<gameObject> objectA, std::shared_ptr<gameObject> objectB);
        static bool intersects(std::shared_ptr<gameObject> objectA, camera objectB);

        static bool contains(std::shared_ptr<gameObject> objectA, const glm::vec2& point);

        static bool intersectsAll(std::shared_ptr<gameObject> objectA, std::vector<std::shared_ptr<gameObject>>& objects);
        static std::shared_ptr<gameObject> intersectsFirst(std::shared_ptr<gameObject> objectA, std::vector<std::shared_ptr<gameObject>>& objects);
    };
}
