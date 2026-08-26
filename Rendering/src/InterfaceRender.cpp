#include "InterfaceRender.h"

#include "DrawCommand.h"
#include "IndexBuffer.h"
#include "PushConstants.h"
#include <ranges>
#include <vector>

void UInterfaceRender::Init()
{
	frameContextBuffer_.Init();
	drawDataBuffer_.Init();
	transformBuffer_.Init();
	parametersBuffer_.Init();
}

void UInterfaceRender::Cleanup() const
{
	frameContextBuffer_.Cleanup();
	drawDataBuffer_.Cleanup();
	transformBuffer_.Cleanup();
	parametersBuffer_.Cleanup();
}

void UInterfaceRender::UpdateBuffers(std::span<UDrawCommand const> drawCommands) const
{
	frameContextBuffer_.UpdateBuffer(
		  {}
		, MakeFrameContextAddresses()
		, MakeFrameContextTargets()
		, {}
		, {}
		, {}
	);

	drawDataBuffer_.UpdateBuffer(MakeDrawDataBuffer(drawCommands));
	transformBuffer_.UpdateBuffer(MakeTransformBuffer(drawCommands));
	parametersBuffer_.UpdateBuffer(MakeParametersBuffer(drawCommands));
}

void UInterfaceRender::CmdDraw(
	  VkCommandBuffer commandBuffer
	, VkPipelineLayout pipelineLayout
	, std::span<UDrawCommand const> drawCommands
	, UIndexBuffer const& indexBuffer
) const
{
	CmdDrawFrameInterface(commandBuffer, pipelineLayout, drawCommands, indexBuffer);
}

UFrameContextAddresses UInterfaceRender::MakeFrameContextAddresses() const
{
	return {
		.drawDataBufferAddress = drawDataBuffer_.GetAddress(),
		.materialBufferAddress = {},
		.transformBufferAddress = transformBuffer_.GetAddress(),
		.parametersBufferAddress = parametersBuffer_.GetAddress(),

		.directionalLightBufferAddress = {},
		.pointLightBufferAddress = {},

		.clusterAABBBufferAddress = {},
		.clusterGridBufferAddress = {},
		.lightIndexBufferAddress = {},
		.lightCounterBufferAddress = {},
	};
}

UFrameContextTargets UInterfaceRender::MakeFrameContextTargets() const
{
	return UFrameContextTargets{
		.gBufferDepth = {},
		.gBufferAlbedo = {},
		.gBufferNormal = {},
		.gBufferORM = {},
		.colorTarget = {},
	};
}

std::vector<UDrawDataBufferData> UInterfaceRender::MakeDrawDataBuffer(std::span<UDrawCommand const> drawCommands) const
{
	return drawCommands
		| std::views::transform([](UDrawCommand const& drawCommand) {
			return UDrawDataBufferData{
				.materialIndex = drawCommand.drawIndex,
				.transformIndex = drawCommand.drawIndex,
				.parametersIndex = drawCommand.drawIndex,
				.vertexBufferAddress = drawCommand.vertexBufferAddress,
			};
		})
		| std::ranges::to<std::vector>();
}

std::vector<UTransformBufferData> UInterfaceRender::MakeTransformBuffer(std::span<UDrawCommand const> drawCommands) const
{
	return drawCommands
		| std::views::transform([](UDrawCommand const& drawCommand) {
			return UTransformBufferData{
				.modelMatrix = drawCommand.modelMatrix,
				.normalMatrix = drawCommand.normalMatrix,
			};
		})
		| std::ranges::to<std::vector>();
}

std::vector<UParametersBufferData> UInterfaceRender::MakeParametersBuffer(std::span<UDrawCommand const> drawCommands) const
{
	return drawCommands
		| std::views::transform([](UDrawCommand const& drawCommand) {
			return UParametersBufferData{
				.scalars = drawCommand.scalars,
				.vectors = drawCommand.vectors,
				.textures = drawCommand.textures,
			};
		})
		| std::ranges::to<std::vector>();
}

void UInterfaceRender::CmdDrawFrameInterface(
	  VkCommandBuffer commandBuffer
	, VkPipelineLayout pipelineLayout
	, std::span<UDrawCommand const> drawCommands
	, UIndexBuffer const& indexBuffer
) const
{
	VkPipeline currentPipeline{ VK_NULL_HANDLE };

	indexBuffer.CmdBind(commandBuffer);
	
	for (auto const& drawCommand : drawCommands)
	{
		if (currentPipeline != drawCommand.pipeline)
		{
			currentPipeline = drawCommand.pipeline;
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, drawCommand.pipeline);
		}

		CmdPushConstants(commandBuffer, pipelineLayout, drawCommand.drawIndex, 0);

		vkCmdSetScissor(commandBuffer, 0, 1, &drawCommand.scissor);

		vkCmdDrawIndexed(commandBuffer
			, drawCommand.indexCount
			, 1
			, drawCommand.firstIndex
			, 0
			, 0
		);
	}
}

void UInterfaceRender::CmdPushConstants(
	  VkCommandBuffer commandBuffer
	, VkPipelineLayout pipelineLayout
	, u32 drawIndex
	, u32 directionalIndex
) const
{
	UPushConstants const pc{
		.frameContextBufferAddress = frameContextBuffer_.GetAddress(),
		.drawIndex = drawIndex,
		.directionalIndex = directionalIndex,
	};

	vkCmdPushConstants(commandBuffer
		, pipelineLayout
		, VK_SHADER_STAGE_ALL
		, 0
		, sizeof(UPushConstants)
		, &pc
	);
}
