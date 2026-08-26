#pragma once
#include "FrameContextBufferData.h"
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vulkan/vulkan_core.h>
class UPipelineResourceManager;
class UFrameContextBuffer final
{
    using Data = UFrameContextBufferData;

public:
    void Init();
    void Cleanup() const;

    void UpdateBuffer(UFrameContextSceneView const& sceneView, UFrameContextAddresses const& addresses, UFrameContextTargets const& targets, u32 directionalLightCount, u32 pointLightCount, u32 samplerIndex) const;
    VkDeviceAddress GetAddress() const;

private:
    void CreateBuffers();

private:
    UAllocatedBuffer dataBuffer_;
};
