#pragma once
#include "frames_in_flight.h"
#include <array>
#include <kotono_common/types.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class SParametersBuffer final
{
public:
    struct Parameters
    {
        std::array<f32, 16>         scalars;
        std::array<glm::vec4, 16>   vectors;
        std::array<u32, 16>         textures;
    };

    struct FrameData
    {
        VkBuffer buffer;
        VmaAllocation allocation;
        Parameters* mapped;
        VkDeviceAddress bda;
    };

public:
    void Init();
    void Cleanup() const;

    u32 RegisterParameters();
    void UnregisterParameters(const u32 index);
    void UpdateParameters(const u32 index, const Parameters& transform);

    void UpdateBuffer(const u32 frameIndex);
    VkDeviceAddress GetAddress(const u32 frameIndex) const;

private:
    void CreateBuffers();
    u32 FindParametersSlot();

private:
    KtFramesInFlightArray<FrameData> frameDatas_;

    std::vector<Parameters> parameters_;
    std::vector<u32> freeParametersSlots_;
    u32 parametersCount_;
};

inline SParametersBuffer ParametersBuffer;
