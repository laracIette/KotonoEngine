#pragma once
#include "frames_in_flight.h"
#include "TransformBufferData.h"
#include <kotono_common/types.h>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class STransformBuffer final
{
    using Data = UTransformBufferData;

public:
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

    Data* RegisterTransform();
    void UnregisterTransform(Data* slot);
    u32 GetIndex(const Data* slot) const;

    void UpdateBuffer(const u32 frameIndex);
    VkDeviceAddress GetAddress(const u32 frameIndex) const;

private:
    void CreateBuffers();
    UTransformBufferData* FindTransformSlot();

private:
    KtFramesInFlightArray<FrameData> frameDatas_;

    std::vector<Data> datas_;
    std::vector<Data*> freeDataSlots_;
    u32 dataCount_;
};

inline STransformBuffer TransformBuffer;
