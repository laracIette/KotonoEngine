#include "InterfaceRenderer.h"

#include "DrawCommand.h"
#include "InterfaceRender.h"

UInterfaceRenderer::UInterfaceRenderer(UDevice& device)
	: device_{ device }
	, frameDatas_{ 
		UInterfaceRender{ device },
		UInterfaceRender{ device },
		UInterfaceRender{ device }
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
	, VkPipelineLayout pipelineLayout
	, std::span<UDrawCommand const> drawCommands
	, UIndexBuffer const& indexBuffer
) const
{
	frameDatas_[frameIndex].interfaceRender
		.CmdDraw(commandBuffer, pipelineLayout, drawCommands, indexBuffer);
}
