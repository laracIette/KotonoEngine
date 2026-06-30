#pragma once
#include <kotono_common/types.h>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class SUploadStager final // TODO: move to context, only StagingUpload() used
{
public:
    struct StagingBuffer
    {
        VkBuffer buffer;
        VmaAllocation allocation;
    };

public:
    void Flush();

    void StagingUpload(const void* data, const VkDeviceSize size, VkBuffer dstBuffer, const VkDeviceSize dstOffset);

private:
    void CreateCommandPool();
    void CreateCommandBuffer();
    void CreateFence();

    void BeginRecording() const;
    void EndRecording() const;

    void CreateStagingBuffer(VkBuffer& stagingBuffer, VmaAllocation& stagingAlloc, VmaAllocationInfo& allocationInfo, const VkDeviceSize bufSize);
    void ClearDeletionQueue();

private:
    VkCommandPool commandPool_;
    VkCommandBuffer commandBuffer_;
    VkFence fence_;

    std::vector<StagingBuffer> deletionQueue_;
};

inline SUploadStager UploadStager;
