#include "Barriers.h"
#include <kotono_common/types.h>
#include <vector>

namespace Barriers
{
	void CmdTransitionImages(
		  VkCommandBuffer const commandBuffer
		, std::span<VkImage const> const images
		, VkPipelineStageFlags2 const srcStage
		, VkPipelineStageFlags2 const dstStage
		, VkAccessFlags2 const srcAccess
		, VkAccessFlags2 const dstAccess
		, VkImageLayout const oldLayout
		, VkImageLayout const newLayout
		, VkImageSubresourceRange const subresourceRange
	)
	{
		std::vector<VkImageMemoryBarrier2> barriers{};
		barriers.reserve(images.size());

		for (u32 i{ 0 }; i < images.size(); ++i)
		{
			barriers.push_back({
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
				.srcStageMask = srcStage,
				.srcAccessMask = srcAccess,
				.dstStageMask = dstStage,
				.dstAccessMask = dstAccess,
				.oldLayout = oldLayout,
				.newLayout = newLayout,
				.image = images[i],
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

	void CmdTransitionCompute(
		  VkCommandBuffer const commandBuffer
		, VkPipelineStageFlags2 const srcStage
		, VkPipelineStageFlags2 const dstStage
		, VkAccessFlags2 const srcAccess
		, VkAccessFlags2 const dstAccess
	)
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