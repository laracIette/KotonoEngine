#include "LightBuffer.h"
#include <assert.h>
#include <kotono_platform/Context.h>

static constexpr u32 MAX_LIGHTS{ 32 };

void GLightBuffer::Init()
{
    const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(UPointLight) * MAX_LIGHTS,
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

u32 GLightBuffer::RegisterPointLight(const UPointLight& pointLight)
{
    Context.StagingUpload(&pointLight
        , sizeof(UPointLight)
        , buffer_
        , pointLightCount_ * sizeof(UPointLight)
    );
    assert(pointLightCount_ < MAX_LIGHTS);
    return pointLightCount_++;
}

void GLightBuffer::RegisterDirectionalLight(const UDirectionalLight& directionalLight)
{
    directionalLight_ = directionalLight;
}

VkDeviceAddress GLightBuffer::GetAddress() const
{
    return bda_;
}

u32 GLightBuffer::GetPointLightCount() const
{
    return pointLightCount_;
}

const UDirectionalLight& GLightBuffer::GetDirectionalLight() const
{
    return directionalLight_;
}
