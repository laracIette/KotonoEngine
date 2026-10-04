#pragma once
#include "frames_in_flight.h"
#include "InterfaceRender.h"
#include <kotono_common/types.h>
#include <span>
struct UDrawCommand;
class UDevice;
class UPipelineResourceManager;
class UInterfaceRenderer final
{
public:
	struct FrameData
	{
		UInterfaceRender interfaceRender;
	};

public:
	explicit UInterfaceRenderer(UDevice& device, UPipelineResourceManager& pipelineResourceManager);

	void Init();
	void Cleanup() const;

	void UpdateInterfaceBuffers(std::span<UDrawCommand const> drawCommands, u32 frameIndex);
	void CmdDrawInterface(VkCommandBuffer commandBuffer, u32 frameIndex, std::span<UDrawCommand const> drawCommands, UIndexBuffer const& indexBuffer) const;

private:
	UDevice& device_; 
	UPipelineResourceManager& pipelineResourceManager_;

	UFramesInFlightArray<FrameData> frameDatas_;
};