#include "Barriers.h"
#include <vector>

namespace Barriers
{
	void CmdTransitionImages(VkCommandBuffer commandBuffer
		, const VkImage* image
		, const u32 count
		, const VkPipelineStageFlags2 srcStage
		, const VkPipelineStageFlags2 dstStage
		, const VkAccessFlags2 srcAccess
		, const VkAccessFlags2 dstAccess
		, const VkImageLayout oldLayout
		, const VkImageLayout newLayout
		, const VkImageSubresourceRange subresourceRange)
	{
		std::vector<VkImageMemoryBarrier2> barriers{};
		barriers.reserve(count);
		for (u32 i{ 0 }; i < count; ++i)
		{
			barriers.push_back({
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
				.srcStageMask = srcStage,
				.srcAccessMask = srcAccess,
				.dstStageMask = dstStage,
				.dstAccessMask = dstAccess,
				.oldLayout = oldLayout,
				.newLayout = newLayout,
				.image = image[i],
				.subresourceRange = subresourceRange
				});
		}
		const VkDependencyInfo dependencyInfo{
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.imageMemoryBarrierCount = static_cast<u32>(barriers.size()),
			.pImageMemoryBarriers = barriers.data(),
		};
		vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
	}

	void CmdTransitionCompute(VkCommandBuffer commandBuffer
		, const VkPipelineStageFlags2 srcStage
		, const VkPipelineStageFlags2 dstStage
		, const VkAccessFlags2 srcAccess
		, const VkAccessFlags2 dstAccess)
	{
		const VkMemoryBarrier2 barrier{
			.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2,
			.srcStageMask = srcStage,
			.srcAccessMask = srcAccess,
			.dstStageMask = dstStage,
			.dstAccessMask = dstAccess,
		};
		const VkDependencyInfo dependencyInfo{
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.memoryBarrierCount = 1,
			.pMemoryBarriers = &barrier,
		};
		vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
	}
}