#pragma once
#include "frames_in_flight.h"
#include "InterfaceRender.h"
#include <kotono_common/types.h>
#include <span>
struct UDrawCommand;
class UDevice;
class UInterfaceRenderer final
{
public:
	struct FrameData
	{
		UInterfaceRender interfaceRender;
	};

public:
	explicit UInterfaceRenderer(UDevice& device);

	void Init();
	void Cleanup() const;

	void UpdateInterfaceBuffers(std::span<UDrawCommand const> drawCommands, u32 frameIndex);
	void CmdDrawInterface(VkCommandBuffer commandBuffer, u32 frameIndex, VkPipelineLayout pipelineLayout, std::span<UDrawCommand const> drawCommands, UIndexBuffer const& indexBuffer) const;

private:
	UDevice& device_;

	UFramesInFlightArray<FrameData> frameDatas_;
};