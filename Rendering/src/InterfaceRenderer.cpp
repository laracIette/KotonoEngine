#include "InterfaceRenderer.h"

#include "DrawCommand.h"
#include "InterfaceRender.h"

void UInterfaceRenderer::Init(VkDevice device, VmaAllocator allocator)
{
	for (auto& frameData : frameDatas_)
	{
		frameData.interfaceRender.Init(device, allocator);
	}
}

void UInterfaceRenderer::Cleanup(VmaAllocator allocator) const
{
	for (auto const& frameData : frameDatas_)
	{
		frameData.interfaceRender.Cleanup(allocator);
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
