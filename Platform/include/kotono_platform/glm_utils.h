#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
inline constexpr glm::vec3 WorldRightVector{ 1.0f, 0.0f, 0.0f };
inline constexpr glm::vec3 WorldUpVector{ 0.0f, 1.0f, 0.0f };
inline constexpr glm::vec3 WorldForwardVector{ 0.0f, 0.0f, -1.0f };

inline glm::mat4 calculate_reverse_z_infinite_perspective(const float fovY, const float aspect, const float zNear) noexcept
{
    const float f{ 1.0f / std::tan(fovY / 2.0f) };

    glm::mat4 result{ 0.0f }; // Initialize all elements to 0

    // Column 0
    result[0][0] = f / aspect;

    // Column 1
    // Vulkan's NDC has +Y pointing down. Assuming a standard Left-Handed View Space 
    // where +Y is up, we must negate this to flip the Y-axis.
    result[1][1] = -f;

    // Column 2
    // Left-Handed view means +Z goes into the screen.
    // This sets w_clip = z_view
    result[2][3] = 1.0f;

    // Column 3
    // This sets z_clip = zNear * w_view (which is zNear * 1)
    result[3][2] = zNear;

    return result;
}