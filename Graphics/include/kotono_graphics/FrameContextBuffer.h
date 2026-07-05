#pragma once
#include "FrameContextBufferData.h"
#include "frames_in_flight.h"
#include <glm/vec3.hpp>
#include <kotono_common/types.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class GFrameContextBuffer final
{
    using Data = UFrameContextBufferData;

public:
    struct CameraData
    {
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 viewProj;
        glm::vec3 viewPos;
    };

    struct FrameData
    {
        VkBuffer buffer;
        VmaAllocation allocation;
        Data* mapped;
        VkDeviceAddress bda;
    };

public:
    void Init();
    void Cleanup() const;

    void SetCameraData(const CameraData& cameraData);

    void UpdateBuffer(const u32 frameIndex);
    VkDeviceAddress GetAddress(const u32 frameIndex) const;

private:
    void CreateBuffers();

private:
    UFramesInFlightArray<FrameData> frameDatas_;

    CameraData cameraData_;
};

inline GFrameContextBuffer FrameContextBuffer;
