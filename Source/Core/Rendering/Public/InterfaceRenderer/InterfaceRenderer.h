#pragma once

#include "Containers/Span.h"
#include "frames_in_flight.h"
#include "InterfaceRender/InterfaceRender.h"
#include "types.h"

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

	void UpdateInterfaceBuffers(USpan<UDrawCommand const> drawCommands, u32 frameIndex) const;
	void CmdDrawInterface(VkCommandBuffer commandBuffer, u32 frameIndex, USpan<UDrawCommand const> drawCommands, UIndexBuffer const& indexBuffer) const;

private:
	UDevice& device_; 
	UPipelineResourceManager& pipelineResourceManager_;

	UFramesInFlightArray<FrameData> frameDatas_;
};