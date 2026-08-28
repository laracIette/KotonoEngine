#pragma once
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
#include <vulkan/vulkan_core.h>
class UDevice;
class UIndexBuffer final
{
public:
	void Init(UDevice& device);
	void Cleanup(UDevice& device) const;

	void CmdBind(VkCommandBuffer commandBuffer) const;

	u32 RegisterIndices(UDevice& device, std::span<u32 const> indices);

private:
	UAllocatedBuffer dataBuffer_;
	u32 indexCount_;
};
