#pragma once
#include "DrawDataBufferData.h"
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
#include <vulkan/vulkan_core.h>
class UDrawDataBuffer final
{
    using Data = UDrawDataBufferData;

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
