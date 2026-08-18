#pragma once
#include "frames_in_flight.h"
#include "TransformBufferData.h"
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
#include <vulkan/vulkan_core.h>
class UTransformBuffer final
{
    using Data = UTransformBufferData;

public:
    void Init();
    void Cleanup() const;

    void UpdateBuffer(std::span<Data const> datas) const;
    VkDeviceAddress GetAddress() const;

private:
    void CreateBuffers();

private:
    UAllocatedBuffer dataBuffer_;
};
