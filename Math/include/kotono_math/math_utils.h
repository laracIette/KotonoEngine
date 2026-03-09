#pragma once
#include <glm/glm.hpp>

constexpr bool is_point_in_rect(const glm::vec2& point, const glm::vec2& position, const glm::vec2& size) noexcept
{
    return glm::all(glm::greaterThanEqual(point, position))
        && glm::all(glm::lessThanEqual(point, position + size));
}

constexpr bool is_overlapping(const glm::vec2& leftPos, const glm::vec2& leftSize, const glm::vec2& rightPos, const glm::vec2& rightSize) noexcept
{
    return leftPos.x < rightPos.x + rightSize.x 
        && leftPos.x + leftSize.x > rightPos.x 
        && leftPos.y < rightPos.y + rightSize.y 
        && leftPos.y + leftSize.y > rightPos.y;
}

constexpr void compute_intersect(const glm::vec2& leftPos, const glm::vec2& leftSize, const glm::vec2& rightPos, const glm::vec2& rightSize, glm::vec2& intersectPos, glm::vec2& intersectSize) noexcept
{
    const glm::vec2 leftEnd{ leftPos + leftSize };
    const glm::vec2 rightEnd{ rightPos + rightSize };
    const glm::vec2 intersectEnd{ glm::min(leftEnd, rightEnd) };

    intersectPos = glm::max(leftPos, rightPos);
    intersectSize = glm::max(glm::vec2(0.0f), intersectEnd - intersectPos);
}