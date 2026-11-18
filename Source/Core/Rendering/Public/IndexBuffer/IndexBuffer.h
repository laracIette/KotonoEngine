#pragma once

#include "AllocatedBuffer.h"
#include "Containers/Span.h"
#include "types.h"

#include <vulkan/vulkan_core.h>

class UDevice;

class UIndexBuffer final
{
public:
	explicit UIndexBuffer(UDevice& device);

	void Init();
	void Cleanup() const;

	void CmdBind(VkCommandBuffer commandBuffer) const;

	u32 RegisterIndices(USpan<u32 const> indices);

private:
	UDevice& device_;

	UAllocatedBuffer dataBuffer_;
	u32 indexCount_;
};
