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
        u32 meshletOffset;
    };

    struct FrameData
    {
        VkBuffer buffer;
        VmaAllocation allocation;
        DrawData* mapped;
        VkDeviceAddress bda;
        u32 drawCount;
    };

public:
    void Init();
    void Cleanup() const;

    u32 RegisterDrawData(const DrawData& drawData);
    void UnregisterDrawData(const u32 index);
    void UpdateDrawData(const DrawData& drawData, const u32 index);

    void UpdateDrawDatas(const u32 frameIndex);
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
