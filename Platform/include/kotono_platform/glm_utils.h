#pragma once
#include <array>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <kotono_common/types.h>

inline constexpr glm::vec3 WorldRightVector{ 1.0f, 0.0f, 0.0f };
inline constexpr glm::vec3 WorldUpVector{ 0.0f, 1.0f, 0.0f };
inline constexpr glm::vec3 WorldForwardVector{ 0.0f, 0.0f, -1.0f };

inline glm::mat4 calculate_reverse_z_infinite_perspective(const f32 fovY, const f32 aspect, const f32 zNear) noexcept
{
    const f32 f{ 1.0f / std::tan(fovY / 2.0f) };

    glm::mat4 result{ 0.0f }; // Initialize all elements to 0

    result[0][0] = f / aspect;
    result[1][1] = -f; // Flip y axis
    result[2][3] = 1.0f; // w_clip = z_view 
    result[3][2] = zNear; // z_clip = zNear * w_view (which is zNear * 1)

    return result;
}

inline std::array<glm::vec3, 8> get_frustum_corners_world_space(const glm::mat4& view
    , const f32 zNear
    , const f32 zFar
    , const f32 fovYRadians
    , const f32 aspect)
{
    std::array<glm::vec3, 8> corners{};

    // Half-dimensions of the near and far planes in view space
    const f32 tanHalfFOV{ std::tan(fovYRadians * 0.5f) };

    const f32 nearHeight{ zNear * tanHalfFOV };
    const f32 nearWidth{ nearHeight * aspect };

    const f32 farHeight{ zFar * tanHalfFOV };
    const f32 farWidth{ farHeight * aspect };

    // Define the 8 corners in view space
    // - Near plane corners
    corners[0] = { -nearWidth, -nearHeight, zNear };
    corners[1] = { nearWidth, -nearHeight, zNear };
    corners[2] = { -nearWidth, nearHeight, zNear };
    corners[3] = { nearWidth, nearHeight, zNear };
    // - Far plane corners
    corners[4] = { -farWidth, -farHeight, zFar };
    corners[5] = { farWidth, -farHeight, zFar };
    corners[6] = { -farWidth, farHeight, zFar };
    corners[7] = { farWidth, farHeight, zFar };

    // Transform the view space corners into world space
    const glm::mat4 invView{ glm::inverse(view) };
    for (auto& corner : corners)
    {
        const glm::vec4 worldPos{ invView * glm::vec4{ corner, 1.0f} };
        corner = glm::vec3{ worldPos };
    }

    return corners;
}

inline glm::mat4 get_light_space_matrix(const glm::vec3& lightDir
    , const glm::mat4& cameraView
    , const f32 zNear
    , const f32 zFar
    , const f32 fovYRadians
    , const f32 aspect)
{
    const auto corners{ get_frustum_corners_world_space(cameraView, zNear, zFar, fovYRadians, aspect) };

    // Calculate the geometric center of the camera frustum
    glm::vec3 center{ 0.0f };
    for (const auto& v : corners)
    {
        center += v;
    }
    center /= static_cast<f32>(corners.size());

    const glm::vec3 normLightDir{ glm::normalize(lightDir) };

    // Fix parallel vector (normalize(0) == NaN)
    glm::vec3 safeUp{ WorldUpVector };
    if (std::abs(glm::dot(normLightDir, WorldUpVector)) > 0.999f)
    {
        // If the light matches your WorldUp vector, fallback to an alternate axis
        safeUp = (std::abs(WorldUpVector.z) > 0.9f) ? glm::vec3{ 0.0f, 1.0f, 0.0f } : glm::vec3{ 0.0f, 0.0f, 1.0f };
    }

    // Create Left-Handed Light View Matrix
    const glm::mat4 lightView{ glm::lookAt(
        center - normLightDir,
        center,
        safeUp
    ) };

    // Transform corners into Light View Space and find the AABB bounds
    f32 minX{ std::numeric_limits<f32>::max() };
    f32 maxX{ std::numeric_limits<f32>::lowest() };
    f32 minY{ std::numeric_limits<f32>::max() };
    f32 maxY{ std::numeric_limits<f32>::lowest() };
    f32 minZ{ std::numeric_limits<f32>::max() };
    f32 maxZ{ std::numeric_limits<f32>::lowest() };
    for (const auto& v : corners)
    {
        const glm::vec4 trf{ lightView * glm::vec4{ v, 1.0f } };
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    // Allow geometry slightly off camera to cast shadows
    constexpr f32 Z_PADDING{ 5.0f };
    f32 nearDistance = minZ - Z_PADDING;
    f32 farDistance = maxZ + Z_PADDING;

    // Reverse Y and reverse Z 
    const glm::mat4 lightProjection{ glm::ortho(minX, maxX, maxY, minY, farDistance, nearDistance) };

    return lightProjection * lightView;
}
