#include "LightBuffer.h"
#include <assert.h>
#include <kotono_platform/Context.h>

static constexpr u32 MAX_LIGHTS{ 32 };

void GLightBuffer::Init()
{
    const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(Light) * MAX_LIGHTS,
        .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    };
    const VmaAllocationCreateInfo allocInfo{
        .usage = VMA_MEMORY_USAGE_GPU_ONLY
    };
    vmaCreateBuffer(Context.GetAllocator()
        , &bufInfo
        , &allocInfo
        , &buffer_
        , &allocation_
        , nullptr
    );

    const VkBufferDeviceAddressInfo addrInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = buffer_,
    };
    bda_ = vkGetBufferDeviceAddress(Context.GetDevice(), &addrInfo);
}

void GLightBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), buffer_, allocation_);
}

u32 GLightBuffer::RegisterLight(const Light& light)
{
    Context.StagingUpload(&light
        , sizeof(Light)
        , buffer_
        , lightCount_ * sizeof(Light)
    );
    assert(lightCount_ < MAX_LIGHTS);
    return lightCount_++;
}

VkDeviceAddress GLightBuffer::GetAddress() const
{
    return bda_;
}

u32 GLightBuffer::GetLightCount() const
{
    return lightCount_;
}
