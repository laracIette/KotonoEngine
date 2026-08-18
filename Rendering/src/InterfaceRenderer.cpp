#include "InterfaceRenderer.h"

#include "DrawCommand.h"
#include "InterfaceRender.h"

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
	auto const& interfaceRender{ frameDatas_[frameIndex].interfaceRender };
	interfaceRender.UpdateBuffers(drawCommands);
}

void UInterfaceRenderer::CmdDrawInterface(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands, u32 frameIndex) const
{
	auto const& interfaceRender{ frameDatas_[frameIndex].interfaceRender };
	interfaceRender.CmdDraw(commandBuffer, drawCommands);
}
