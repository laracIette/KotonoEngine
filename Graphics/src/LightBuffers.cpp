#include "LightBuffers.h"
#include <assert.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

static constexpr u32 MAX_DIRECTIONAL_LIGHTS{ 16 };
static constexpr u32 MAX_POINT_LIGHTS{ 1024 };

void GLightBuffers::Init()
{
    CreateBuffer(directionalLightBuffer_, sizeof(UDirectionalLight) * MAX_DIRECTIONAL_LIGHTS);
    CreateBuffer(pointLightBuffer_, sizeof(UPointLight) * MAX_POINT_LIGHTS);
}

void GLightBuffers::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), directionalLightBuffer_.buffer, directionalLightBuffer_.allocation);
    vmaDestroyBuffer(Context.GetAllocator(), pointLightBuffer_.buffer, pointLightBuffer_.allocation);
}

u32 GLightBuffers::RegisterDirectionalLight(const UDirectionalLight& directionalLight)
{
    return Register(directionalLightBuffer_, &directionalLight, sizeof(UDirectionalLight), MAX_DIRECTIONAL_LIGHTS);
}

u32 GLightBuffers::RegisterPointLight(const UPointLight& pointLight)
{
    return Register(pointLightBuffer_, &pointLight, sizeof(UPointLight), MAX_POINT_LIGHTS);
}

VkDeviceAddress GLightBuffers::GetDirectionalLightAddress() const
{
    return directionalLightBuffer_.bda;
}

VkDeviceAddress GLightBuffers::GetPointLightAddress() const
{
    return pointLightBuffer_.bda;
}

u32 GLightBuffers::GetDirectionalLightCount() const
{
    return directionalLightBuffer_.count;
}

u32 GLightBuffers::GetPointLightCount() const
{
    return pointLightBuffer_.count;
}

void GLightBuffers::CreateBuffer(LightBuffer& lightBuffer, const VkDeviceSize size)
{
    const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    };
    const VmaAllocationCreateInfo allocInfo{
        .usage = VMA_MEMORY_USAGE_GPU_ONLY
    };
    VK_CHECK_THROW(
        vmaCreateBuffer(Context.GetAllocator()
            , &bufInfo
            , &allocInfo
            , &lightBuffer.buffer
            , &lightBuffer.allocation
            , nullptr
        ),
        "failed to create buffer!"
    );

    const VkBufferDeviceAddressInfo addrInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = lightBuffer.buffer,
    };
    lightBuffer.bda = vkGetBufferDeviceAddress(Context.GetDevice(), &addrInfo);
}

u32 GLightBuffers::Register(LightBuffer& lightBuffer, const void* data, const VkDeviceSize size, const u32 maxCount)
{
    Context.StagingUpload(data
        , size
        , lightBuffer.buffer
        , lightBuffer.count * size
    );
    assert(lightBuffer.count < maxCount);
    return lightBuffer.count++;
}
