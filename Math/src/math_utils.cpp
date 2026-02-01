#include "math_utils.h"
#include <glm/glm.hpp>

bool is_point_in_rect(const glm::vec2& point, const glm::vec2& position, const glm::vec2& size) noexcept
{
    return glm::all(glm::greaterThanEqual(point, position)) 
        && glm::all(glm::lessThanEqual(point, position + size));
}
