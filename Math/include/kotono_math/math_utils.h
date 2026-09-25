#pragma once
#include <cmath>
#include <concepts>
#include <glm/common.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/vector_relational.hpp>

inline constexpr glm::vec3 WorldRightVector{ 1.0f, 0.0f, 0.0f };
inline constexpr glm::vec3 WorldUpVector{ 0.0f, 1.0f, 0.0f };
inline constexpr glm::vec3 WorldForwardVector{ 0.0f, 0.0f, -1.0f };

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
inline T round(T v, int digits) noexcept
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

inline glm::mat4 calculate_reverse_z_infinite_perspective(float fovY, float aspect, float zNear) noexcept
{
    float const f{ 1.0f / std::tan(fovY / 2.0f) };

    glm::mat4 result{ 0.0f }; // Initialize all elements to 0

    result[0][0] = f / aspect;
    result[1][1] = -f; // Flip y axis
    result[2][3] = 1.0f; // w_clip = z_view 
    result[3][2] = zNear; // z_clip = zNear * w_view (which is zNear * 1)

    return result;
}

constexpr glm::vec3 right_vector(glm::quat const& quat)
{
    return quat * WorldRightVector;
}

constexpr glm::vec3 up_vector(glm::quat const& quat)
{
    return quat * WorldUpVector;
}

constexpr glm::vec3 forward_vector(glm::quat const& quat)
{
    return quat * WorldForwardVector;
}
