#pragma once
#include "frames_in_flight.h"
#include <kotono_common/types.h>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class SDrawDataBuffer final
{
public:
    struct DrawData
    {
        u32 materialIndex;
        u32 transformIndex; 
        u32 parametersIndex; 
        u32 meshletOffset;
    };

    struct FrameData
    {
        VkBuffer buffer;
        VmaAllocation allocation;
        DrawData* mapped;
        VkDeviceAddress bda;
    };

public:
    void Init();
    void Cleanup() const;

    u32 RegisterDrawData();
    void UnregisterDrawData(const u32 index);
    void UpdateDrawData(const u32 index, const DrawData& drawData);

    void UpdateBuffer(const u32 frameIndex);
    VkDeviceAddress GetAddress(const u32 frameIndex) const;

private:
    void CreateBuffers();
    u32 FindDrawDataSlot();

private:
    KtFramesInFlightArray<FrameData> frameDatas_;

    std::vector<DrawData> drawDatas_;
    std::vector<u32> freeDrawDataSlots_;
    u32 drawDatasCount_;
};

inline SDrawDataBuffer DrawDataBuffer;
