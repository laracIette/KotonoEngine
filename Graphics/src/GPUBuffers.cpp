#include "GPUBuffers.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

static constexpr u32 CLUSTER_AABB_COUNT{ 16 * 9 * 24 };

void GGPUBuffers::Init()
{
    CreateBuffer(clusterAABBBuffer_
        , sizeof(ClusterAABB) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
    );

    CreateBuffer(clusterGridBuffer_
        , sizeof(ClusterGrid) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
    );

    CreateBuffer(lightIndexBuffer_
        , sizeof(u32) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
    );

    CreateBuffer(lightCounterBuffer_
        , sizeof(u32)
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
    );
}

void GGPUBuffers::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), clusterAABBBuffer_.buffer, clusterAABBBuffer_.allocation);
    vmaDestroyBuffer(Context.GetAllocator(), clusterGridBuffer_.buffer, clusterGridBuffer_.allocation);
    vmaDestroyBuffer(Context.GetAllocator(), lightIndexBuffer_.buffer, lightIndexBuffer_.allocation);
    vmaDestroyBuffer(Context.GetAllocator(), lightCounterBuffer_.buffer, lightCounterBuffer_.allocation);
}

VkDeviceAddress GGPUBuffers::GetClusterAABBAddress() const
{
    return clusterAABBBuffer_.bda;
}

VkDeviceAddress GGPUBuffers::GetClusterGridAddress() const
{
    return clusterGridBuffer_.bda;
}

VkDeviceAddress GGPUBuffers::GetLightIndexAddress() const
{
    return lightIndexBuffer_.bda;
}

VkDeviceAddress GGPUBuffers::GetLightCounterAddress() const
{
    return lightCounterBuffer_.bda;
}

VkBuffer GGPUBuffers::GetLightCounterBuffer() const
{
    return lightCounterBuffer_.buffer;
}

void GGPUBuffers::CreateBuffer(AllocatedBuffer& allocatedBuffer, const VkDeviceSize size, const VkBufferUsageFlags usage)
{
    const VkBufferCreateInfo bufInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage,
    };
    const VmaAllocationCreateInfo allocInfo{
        .usage = VMA_MEMORY_USAGE_GPU_ONLY
    };
    VK_CHECK_THROW(
        vmaCreateBuffer(Context.GetAllocator()
            , &bufInfo
            , &allocInfo
            , &allocatedBuffer.buffer
            , &allocatedBuffer.allocation
            , nullptr
        ),
        "failed to allocate buffer!"
    );

    const VkBufferDeviceAddressInfo addrInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = allocatedBuffer.buffer,
    };
    allocatedBuffer.bda = vkGetBufferDeviceAddress(Context.GetDevice(), &addrInfo);
}
