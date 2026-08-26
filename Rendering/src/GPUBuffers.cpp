#include "GPUBuffers.h"

static constexpr u32 CLUSTER_AABB_COUNT{ 16 * 9 * 24 };

void UGPUBuffers::Init(VkDevice device, VmaAllocator allocator)
{
    clusterAABBBuffer_.Create(device, allocator
        , sizeof(ClusterAABB) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
   
    clusterGridBuffer_.Create(device, allocator
        , sizeof(ClusterGrid) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
    lightIndexBuffer_.Create(device, allocator
        , sizeof(u32) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
    lightCounterBuffer_.Create(device, allocator
        , sizeof(u32)
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
        , 0
    );
}

void UGPUBuffers::Cleanup(VmaAllocator allocator) const
{
    clusterAABBBuffer_.Cleanup(allocator);
    clusterGridBuffer_.Cleanup(allocator);
    lightIndexBuffer_.Cleanup(allocator);
    lightCounterBuffer_.Cleanup(allocator);
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
