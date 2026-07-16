#pragma once
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vulkan/vulkan_core.h>
#include <span>
class GIndexBuffer final
{
public:
	void Init();
	void Cleanup() const;

	void CmdBind(VkCommandBuffer commandBuffer) const;

	u32 RegisterIndices(const std::span<u32> indices);

private:
	UAllocatedBuffer dataBuffer_;
	u32 indexCount_;
};

inline GIndexBuffer IndexBuffer;
