#pragma once
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
#include <vulkan/vulkan_core.h>
class UIndexBuffer final
{
public:
	void Init(VkDevice device, VmaAllocator allocator);
	void Cleanup(VmaAllocator allocator) const;

	void CmdBind(VkCommandBuffer commandBuffer) const;

	u32 RegisterIndices(std::span<u32 const> indices);

private:
	UAllocatedBuffer dataBuffer_;
	u32 indexCount_;
};
