#include "InterfaceRenderer/InterfaceRenderer.h"

#include "DrawCommand.h"
#include "InterfaceRender/InterfaceRender.h"

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
	for (auto& [interfaceRender] : frameDatas_)
	{
		interfaceRender.Init();
	}
}

void UInterfaceRenderer::Cleanup() const
{
	for (auto const& [interfaceRender] : frameDatas_)
	{
		interfaceRender.Cleanup();
	}
}

void UInterfaceRenderer::UpdateInterfaceBuffers(USpan<UDrawCommand const> drawCommands, u32 frameIndex) const
{
	frameDatas_[frameIndex].interfaceRender
		.UpdateBuffers(drawCommands);
}

void UInterfaceRenderer::CmdDrawInterface(
	  VkCommandBuffer commandBuffer
	, u32 frameIndex
	, USpan<UDrawCommand const> drawCommands
	, UIndexBuffer const& indexBuffer
) const
{
	frameDatas_[frameIndex].interfaceRender
		.CmdDraw(commandBuffer, drawCommands, indexBuffer);
}
