#pragma once
#include "FrameContextBufferData.h"
#include "frames_in_flight.h"
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vulkan/vulkan_core.h>
class GFrameContextBuffer final
{
    using Data = UFrameContextBufferData;

public:
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

    void RegisterGBufferTextures();
    void UnregisterGBufferTextures();

    void UpdateBuffer(const u32 frameIndex);
    VkDeviceAddress GetAddress(const u32 frameIndex) const;

private:
    void CreateBuffers();

private:
    UFramesInFlightArray<FrameData> frameDatas_;
};

inline GFrameContextBuffer FrameContextBuffer;
