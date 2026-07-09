#pragma once
#include <array>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
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

inline std::array<glm::vec4, 8> get_frustum_corners_world_space(const glm::mat4& proj, const glm::mat4& view)
{
    const auto inv{ glm::inverse(proj * view) };

    std::array<glm::vec4, 8> frustumCorners{};
    size index{ 0 };
    for (u32 x{ 0 }; x < 2; ++x)
    {
        for (u32 y{ 0 }; y < 2; ++y)
        {
            for (u32 z{ 0 }; z < 2; ++z)
            {
                // NDC coordinates range from -1 to 1
                const glm::vec4 pt{ 
                    inv * glm::vec4{
                        2.0f * x - 1.0f,
                        2.0f * y - 1.0f,
                        2.0f * z - 1.0f,
                        1.0f
                    } 
                };

                // Perspective divide is crucial here!
                frustumCorners[index++] = pt / pt.w;
            }
        }
    }

    return frustumCorners;
}

inline glm::mat4 get_light_space_matrix(const glm::mat4& cameraProj, const glm::mat4& cameraView, const glm::vec3& lightDir)
{
    const auto corners{ get_frustum_corners_world_space(cameraProj, cameraView) };
    
    // Find the center of the frustum
    glm::vec3 center{ 0, 0, 0 };
    for (const auto& v : corners)
    {
        center += glm::vec3{ v };
    }
    center /= corners.size();

    // Create the Light View Matrix
    const glm::mat4 lightView{ glm::lookAt(
        center - (glm::normalize(lightDir) * 1.0f), // Arbitrary step back to position outside of scene, scale doesn't matter yet
        center,
        WorldUpVector
    ) };

    // Transform corners to Light Space and find min/max boundaries
    f32 minX{ std::numeric_limits<f32>::max() };
    f32 maxX{ std::numeric_limits<f32>::lowest() };
    f32 minY{ std::numeric_limits<f32>::max() };
    f32 maxY{ std::numeric_limits<f32>::lowest() };
    f32 minZ{ std::numeric_limits<f32>::max() };
    f32 maxZ{ std::numeric_limits<f32>::lowest() };

    for (const auto& v : corners)
    {
        const auto trf{ lightView * v };
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    // Extend the Z-bounds to capture objects behind the camera (objects slightly off-camera cast shadows)
    constexpr f32 Z_MULTIPLIER{ 10.0f };

    if (minZ < 0)
    {
        minZ *= Z_MULTIPLIER;
    }
    else
    {
        minZ /= Z_MULTIPLIER;
    }
    if (maxZ < 0)
    {
        maxZ /= Z_MULTIPLIER;
    }
    else
    {
        maxZ *= Z_MULTIPLIER;
    }

    // 6. Create the Orthographic Projection Matrix
    const glm::mat4 lightProjection{ glm::ortho(minX, maxX, minY, maxY, minZ, maxZ) };

    // 7. Combine and return
    return lightProjection * lightView;
}
