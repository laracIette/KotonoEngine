#include "MaterialBuffer.h"
#include <kotono_platform/Context.h>

static constexpr u32 MAX_MATERIALS{ 4096 };

void SMaterialBuffer::Init()
{
    const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(Material) * MAX_MATERIALS,
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

void SMaterialBuffer::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), buffer_, allocation_);
}

u32 SMaterialBuffer::RegisterMaterial(const Material& material)
{
    Context.StagingUpload(&material
        , sizeof(Material)
        , buffer_
        , materialCount_ * sizeof(Material)
    );

    return materialCount_++;
}

VkDeviceAddress SMaterialBuffer::GetAddress() const
{
    return bda_;
}
