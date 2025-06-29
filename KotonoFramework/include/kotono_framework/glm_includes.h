#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace std
{
    template<>
    struct hash<glm::mat4>
    {
        size_t operator()(const glm::mat4& mat) const
        {
            const float* data = glm::value_ptr(mat);
            size_t h = 0;
            for (int i = 0; i < 16; ++i)
            {
                h ^= hash<float>()(data[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };
}