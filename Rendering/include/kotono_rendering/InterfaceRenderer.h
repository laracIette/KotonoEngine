#pragma once
#include "frames_in_flight.h"
#include "InterfaceRender.h"
#include <kotono_common/types.h>
#include <span>
struct UDrawCommand;
class UInterfaceRenderer final
{
public:
	struct FrameData
	{
		UInterfaceRender interfaceRender;
	};
public:
	void Init(VkDevice device, VmaAllocator allocator);
	void Cleanup(VmaAllocator allocator) const;

	void UpdateInterfaceBuffers(std::span<UDrawCommand const> drawCommands, u32 frameIndex);
	void CmdDrawInterface(VkCommandBuffer commandBuffer, u32 frameIndex, VkPipelineLayout pipelineLayout, std::span<UDrawCommand const> drawCommands, UIndexBuffer const& indexBuffer) const;

private:
	UFramesInFlightArray<FrameData> frameDatas_;
};