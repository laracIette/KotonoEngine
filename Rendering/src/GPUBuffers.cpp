#include "GPUBuffers.h"
#include <kotono_platform/Context.h>

static constexpr u32 CLUSTER_AABB_COUNT{ 16 * 9 * 24 };

void UGPUBuffers::Init()
{
    Context.CreateBuffer(clusterAABBBuffer_
        , sizeof(ClusterAABB) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
   
    Context.CreateBuffer(clusterGridBuffer_
        , sizeof(ClusterGrid) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
    Context.CreateBuffer(lightIndexBuffer_
        , sizeof(u32) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
    Context.CreateBuffer(lightCounterBuffer_
        , sizeof(u32)
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
        , 0
    );
}

void UGPUBuffers::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), clusterAABBBuffer_.buffer, clusterAABBBuffer_.allocation);

    vmaDestroyBuffer(Context.GetAllocator(), clusterGridBuffer_.buffer, clusterGridBuffer_.allocation);
    vmaDestroyBuffer(Context.GetAllocator(), lightIndexBuffer_.buffer, lightIndexBuffer_.allocation);
    vmaDestroyBuffer(Context.GetAllocator(), lightCounterBuffer_.buffer, lightCounterBuffer_.allocation);
}

VkDeviceAddress UGPUBuffers::GetClusterAABBAddress() const
{
    return clusterAABBBuffer_.bda;
}

VkDeviceAddress UGPUBuffers::GetClusterGridAddress() const
{
    return clusterGridBuffer_.bda;
}

VkDeviceAddress UGPUBuffers::GetLightIndexAddress() const
{
    return lightIndexBuffer_.bda;
}

VkDeviceAddress UGPUBuffers::GetLightCounterAddress() const
{
    return lightCounterBuffer_.bda;
}

VkBuffer UGPUBuffers::GetLightCounterBuffer() const
{
    return lightCounterBuffer_.buffer;
}
