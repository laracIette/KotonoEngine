#include "UploadStager.h"
#include "Context.h"
#include "vk_utils.h"

void SUploadStager::StagingUpload(const void* data
	, const VkDeviceSize dataSize
	, VkBuffer dstBuffer
	, const VkDeviceSize dstOffset)
{
	// Create a temporary host-visible staging buffer
	VkBuffer stagingBuffer;
	VmaAllocation stagingAlloc;
	VmaAllocationInfo allocationInfo;
	CreateStagingBuffer(stagingBuffer, stagingAlloc, allocationInfo, dataSize);

	// Copy CPU data into the staging buffer
	std::memcpy(allocationInfo.pMappedData, data, dataSize);

	// Flush manually to prevent bugs
	vmaFlushAllocation(Context.GetAllocator(), stagingAlloc, 0, VK_WHOLE_SIZE);

	VkCommandBuffer commandBuffer{ Context.BeginSingleTimeCommands() };

	// Record the GPU-side copy
	const VkBufferCopy region{
		.srcOffset = 0,
		.dstOffset = dstOffset,
		.size = dataSize,
	};
	vkCmdCopyBuffer(commandBuffer, stagingBuffer, dstBuffer, 1, &region);

	// Make the copy visible to shaders
	const VkBufferMemoryBarrier2 barrier{
		.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
		.srcStageMask = VK_PIPELINE_STAGE_2_COPY_BIT,
		.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
		.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT,
		.buffer = dstBuffer,
		.offset = dstOffset,
		.size = dataSize,
	};
	const VkDependencyInfo depInfo{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.bufferMemoryBarrierCount = 1,
		.pBufferMemoryBarriers = &barrier,
	};
	vkCmdPipelineBarrier2(commandBuffer, &depInfo);

	Context.EndSingleTimeCommands(commandBuffer);

	// Staging buffer must outlive the command buffer
	deletionQueue_.push_back({ stagingBuffer, stagingAlloc });

	Context.GetEventExecuteSingleTimeCommands().AddListener(this, &SUploadStager::ClearDeletionQueue);
}

void SUploadStager::CreateStagingBuffer(VkBuffer& stagingBuffer
	, VmaAllocation& stagingAlloc
	, VmaAllocationInfo& allocationInfo
	, const VkDeviceSize bufSize)
{
	const VkBufferCreateInfo stagingInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = bufSize,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	};
	const VmaAllocationCreateInfo allocInfo{
		.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT
			| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		.usage = VMA_MEMORY_USAGE_AUTO,
	};

	VK_CHECK_THROW(
		vmaCreateBuffer(Context.GetAllocator()
			, &stagingInfo
			, &allocInfo
			, &stagingBuffer
			, &stagingAlloc
			, &allocationInfo
		),
		"failed to create buffer!"
	);
}

void SUploadStager::ClearDeletionQueue()
{
	if (deletionQueue_.empty())
	{
		return;
	}

	for (auto& stagingBuffer : deletionQueue_)
	{
		vmaDestroyBuffer(Context.GetAllocator(), stagingBuffer.buffer, stagingBuffer.allocation);
	}
	deletionQueue_.clear();
}
