#include "UploadStager.h"
#include "Context.h"
#include "vk_utils.h"

void SUploadStager::Flush()
{
	if (deletionQueue_.empty())
	{
		return;
	}

	EndRecording();

	const VkCommandBufferSubmitInfo cmdInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		.commandBuffer = commandBuffer_,
	};
	const VkSubmitInfo2 submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &cmdInfo,
	};
	VK_CHECK_THROW(
		vkQueueSubmit2(Context.GetGraphicsQueue(), 1, &submitInfo, fence_),
		"failed submitting the queue!"
	);

	// Stall until done, acceptable at load time, not during a frame
	VK_CHECK_THROW(
		vkWaitForFences(Context.GetDevice(), 1, &fence_, VK_TRUE, UINT64_MAX),
		"failed waiting for fences!"
	);
	vkResetFences(Context.GetDevice(), 1, &fence_);

	for (auto& stagingBuffer : deletionQueue_)
	{
		vmaDestroyBuffer(Context.GetAllocator(), stagingBuffer.buffer, stagingBuffer.allocation);
	}
	deletionQueue_.clear();

	vkResetCommandBuffer(commandBuffer_, 0);
	BeginRecording();
}

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

void SUploadStager::CreateCommandPool()
{
	const KtQueueFamilyIndices queueFamilyIndices{ Context.FindQueueFamilies(Context.GetPhysicalDevice()) };

	const VkCommandPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
	};
	VK_CHECK_THROW(
		vkCreateCommandPool(Context.GetDevice(), &poolInfo, nullptr, &commandPool_),
		"failed to create command pool!"
	);
}

void SUploadStager::CreateCommandBuffer()
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool_,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Context.GetDevice(), &allocInfo, &commandBuffer_),
		"failed to allocate command buffers!"
	);
}

void SUploadStager::CreateFence()
{
	const VkFenceCreateInfo fenceInfo{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
	};
	VK_CHECK_THROW(
		vkCreateFence(Context.GetDevice(), &fenceInfo, nullptr, &fence_),
		"failed to create fence!"
	);
}

void SUploadStager::BeginRecording() const
{
	const VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};
	VK_CHECK_THROW(
		vkBeginCommandBuffer(commandBuffer_, &beginInfo),
		"failed to begin recording command buffer!"
	);
}

void SUploadStager::EndRecording() const
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer_),
		"failed to end recording command buffer!"
	);
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

	vmaCreateBuffer(Context.GetAllocator()
		, &stagingInfo
		, &allocInfo
		, &stagingBuffer
		, &stagingAlloc
		, &allocationInfo
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
