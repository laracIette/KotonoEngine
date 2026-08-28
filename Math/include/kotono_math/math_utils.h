#pragma once
#include <concepts>
#include <glm/common.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/vector_relational.hpp>
#include <math.h>

constexpr bool is_point_in_rect(glm::vec2 const& point, glm::vec2 const& position, glm::vec2 const& size) noexcept
{
    return glm::all(glm::greaterThanEqual(point, position))
        && glm::all(glm::lessThanEqual(point, position + size));
}

constexpr bool is_overlapping(glm::vec2 const& leftPos, glm::vec2 const& leftSize, glm::vec2 const& rightPos, glm::vec2 const& rightSize) noexcept
{
    return leftPos.x < rightPos.x + rightSize.x 
        && leftPos.x + leftSize.x > rightPos.x 
        && leftPos.y < rightPos.y + rightSize.y 
        && leftPos.y + leftSize.y > rightPos.y;
}

constexpr void compute_intersect(glm::vec2 const& leftPos, glm::vec2 const& leftSize, glm::vec2 const& rightPos, glm::vec2 const& rightSize, glm::vec2& intersectPos, glm::vec2& intersectSize) noexcept
{
    glm::vec2 const leftEnd{ leftPos + leftSize };
    glm::vec2 const rightEnd{ rightPos + rightSize };
    glm::vec2 const intersectEnd{ glm::min(leftEnd, rightEnd) };

    intersectPos = glm::max(leftPos, rightPos);
    intersectSize = glm::max(glm::vec2{ 0.0f }, intersectEnd - intersectPos);
}

constexpr glm::vec2 px_to_ndc_size(glm::vec2 const& px, glm::vec2 const& bounds) noexcept
{
    return px / glm::vec2{ bounds } * glm::vec2{ 1.0f, -1.0f } * 2.0f;
}

constexpr glm::vec2 px_to_ndc_pos(glm::vec2 const& px, glm::vec2 const& bounds) noexcept
{
    return px / glm::vec2{ bounds } * 2.0f - 1.0f;
}

template<std::floating_point T>
T round(T v, int digits) noexcept
{
    T const factor{ static_cast<T>(std::pow(T{ 10 }, digits)) };
    return std::round(v * factor) / factor;
}

template<std::floating_point T>
constexpr T clamp01(T v) noexcept
{
    constexpr T min{ 0 };
    constexpr T max{ 1 };
    return std::min(std::max(min, v), max);
}
