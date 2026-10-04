#include "GPUBuffers.h"

#include <kotono_platform/Device.h>

static constexpr u32 CLUSTER_AABB_COUNT{ 16 * 9 * 24 };

UGPUBuffers::UGPUBuffers(UDevice& device)
    : device_{ device }
{
}

void UGPUBuffers::Init()
{
    clusterAABBBuffer_ = device_.CreateAllocatedBuffer(
          sizeof(ClusterAABB) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
   
    clusterGridBuffer_ = device_.CreateAllocatedBuffer(
          sizeof(ClusterGrid) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
    lightIndexBuffer_ = device_.CreateAllocatedBuffer(
          sizeof(u32) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
    lightCounterBuffer_ = device_.CreateAllocatedBuffer(
          sizeof(u32)
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
        , 0
    );
}

void UGPUBuffers::Cleanup() const
{
    device_.CleanupAllocatedBuffer(clusterAABBBuffer_);
    device_.CleanupAllocatedBuffer(clusterGridBuffer_);
    device_.CleanupAllocatedBuffer(lightIndexBuffer_);
    device_.CleanupAllocatedBuffer(lightCounterBuffer_);
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
