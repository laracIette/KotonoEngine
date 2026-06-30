#pragma once
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
    void StagingUpload(const void* data, const VkDeviceSize size, VkBuffer dstBuffer, const VkDeviceSize dstOffset);

private:
    void CreateStagingBuffer(VkBuffer& stagingBuffer, VmaAllocation& stagingAlloc, VmaAllocationInfo& allocationInfo, const VkDeviceSize bufSize);
    void ClearDeletionQueue();

private:
    std::vector<StagingBuffer> deletionQueue_;
};

inline SUploadStager UploadStager;
