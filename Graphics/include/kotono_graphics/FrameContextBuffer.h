#pragma once
#include "FrameContextBufferData.h"
#include "frames_in_flight.h"
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
#include <vulkan/vulkan_core.h>
class GPipelineResourceManager;
class UFrameContextBuffer final
{
    using Data = UFrameContextBufferData;

public:
    struct FrameViews
    {
        VkImageView albedo;
        VkImageView normal;
        VkImageView orm;
        VkImageView depth;
        VkImageView postProcessTarget;
    };

    struct FrameData
    {
        UAllocatedBuffer dataBuffer;

        u32 gBufferAlbedo;
        u32 gBufferNormal;
        u32 gBufferORM;
        u32 gBufferDepth;

        u32 postProcessTarget;
    };

public:
    void Init();
    void Cleanup() const;

    void RegisterTextures(GPipelineResourceManager& pipelineResourceManager, const std::span<const FrameViews> frameViewsArray);
    void UnregisterTextures(GPipelineResourceManager& pipelineResourceManager) const;

    void UpdateBuffer(const u32 frameIndex, const UFrameContextSceneView& sceneView, const UFrameContextAddresses& addresses, const u32 samplerIndex);
    VkDeviceAddress GetAddress(const u32 frameIndex) const;

private:
    void CreateBuffers();

private:
    UFramesInFlightArray<FrameData> frameDatas_;
};
