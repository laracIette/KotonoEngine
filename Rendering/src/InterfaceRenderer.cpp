#include "InterfaceRenderer.h"

#include "DrawCommand.h"
#include "InterfaceRender.h"

UInterfaceRenderer::UInterfaceRenderer(UDevice& device, UPipelineResourceManager& pipelineResourceManager)
	: device_{ device }
	, pipelineResourceManager_{ pipelineResourceManager }
	, frameDatas_{ 
		UInterfaceRender{ device, pipelineResourceManager },
		UInterfaceRender{ device, pipelineResourceManager },
		UInterfaceRender{ device, pipelineResourceManager }
	}
{
}

void UInterfaceRenderer::Init()
{
	for (auto& frameData : frameDatas_)
	{
		frameData.interfaceRender.Init();
	}
}

void UInterfaceRenderer::Cleanup() const
{
	for (auto const& frameData : frameDatas_)
	{
		frameData.interfaceRender.Cleanup();
	}
}

void UInterfaceRenderer::UpdateInterfaceBuffers(std::span<UDrawCommand const> drawCommands, u32 frameIndex)
{
	frameDatas_[frameIndex].interfaceRender
		.UpdateBuffers(drawCommands);
}

void UInterfaceRenderer::CmdDrawInterface(
	  VkCommandBuffer commandBuffer
	, u32 frameIndex
	, std::span<UDrawCommand const> drawCommands
	, UIndexBuffer const& indexBuffer
) const
{
	frameDatas_[frameIndex].interfaceRender
		.CmdDraw(commandBuffer, drawCommands, indexBuffer);
}
