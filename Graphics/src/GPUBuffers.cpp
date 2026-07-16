#include "GPUBuffers.h"
#include <kotono_platform/Context.h>

static constexpr u32 CLUSTER_AABB_COUNT{ 16 * 9 * 24 };

void GGPUBuffers::Init()
{
    Context.CreateBuffer(clusterAABBBuffer_
        , sizeof(ClusterAABB) * CLUSTER_AABB_COUNT
        , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        , 0
    );
    for (auto& frameData : frameDatas_)
    {
        Context.CreateBuffer(frameData.clusterGridBuffer
            , sizeof(ClusterGrid) * CLUSTER_AABB_COUNT
            , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            , 0
        );
        Context.CreateBuffer(frameData.lightIndexBuffer
            , sizeof(u32) * CLUSTER_AABB_COUNT
            , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            , 0
        );
        Context.CreateBuffer(frameData.lightCounterBuffer
            , sizeof(u32)
            , VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
            | VK_BUFFER_USAGE_TRANSFER_DST_BIT
            , 0
        );
    }
}

void GGPUBuffers::Cleanup() const
{
    vmaDestroyBuffer(Context.GetAllocator(), clusterAABBBuffer_.buffer, clusterAABBBuffer_.allocation);
    for (const auto& frameData : frameDatas_)
    {
        vmaDestroyBuffer(Context.GetAllocator(), frameData.clusterGridBuffer.buffer, frameData.clusterGridBuffer.allocation);
        vmaDestroyBuffer(Context.GetAllocator(), frameData.lightIndexBuffer.buffer, frameData.lightIndexBuffer.allocation);
        vmaDestroyBuffer(Context.GetAllocator(), frameData.lightCounterBuffer.buffer, frameData.lightCounterBuffer.allocation);
    }
}

VkDeviceAddress GGPUBuffers::GetClusterAABBAddress() const
{
    return clusterAABBBuffer_.bda;
}

VkDeviceAddress GGPUBuffers::GetClusterGridAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].clusterGridBuffer.bda;
}

VkDeviceAddress GGPUBuffers::GetLightIndexAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].lightIndexBuffer.bda;
}

VkDeviceAddress GGPUBuffers::GetLightCounterAddress(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].lightCounterBuffer.bda;
}

VkBuffer GGPUBuffers::GetLightCounterBuffer(const u32 frameIndex) const
{
    return frameDatas_[frameIndex].lightCounterBuffer.buffer;
}
