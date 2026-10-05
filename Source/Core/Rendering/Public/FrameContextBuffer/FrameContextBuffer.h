#pragma once
#include "FrameContextBufferData.h"
#include <types.h>
#include <AllocatedBuffer.h>
#include <vulkan/vulkan_core.h>

class UDevice;

class UFrameContextBuffer final
{
    using Data = UFrameContextBufferData;

public:
    explicit UFrameContextBuffer(UDevice& device);

    void Init();
    void Cleanup() const;

    void UpdateBuffer(UFrameContextSceneView const& sceneView, UFrameContextAddresses const& addresses, UFrameContextTargets const& targets, u32 directionalLightCount, u32 pointLightCount, u32 samplerIndex) const;
    VkDeviceAddress GetAddress() const;

private:
    UDevice& device_;

    UAllocatedBuffer dataBuffer_;
};
