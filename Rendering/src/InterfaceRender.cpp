#include "InterfaceRender.h"

#include "DrawCommand.h"
#include "DrawDataBufferData.h"
#include "IndexBuffer.h"
#include "ParametersBufferData.h"
#include "PushConstants.h"
#include "TransformBufferData.h"
#include <ranges>
#include <vector>

static constexpr u32 MAX_DRAW_DATAS{ 65536 };

void UInterfaceRender::Init(VkDevice device, VmaAllocator allocator)
{
	frameContextBuffer_.Init(device, allocator);

	drawDataBuffer_.Create(device, allocator
		, sizeof(UDrawDataBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
	transformBuffer_.Create(device, allocator
		, sizeof(UTransformBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
	parametersBuffer_.Create(device, allocator
		, sizeof(UParametersBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
}

void UInterfaceRender::Cleanup(VmaAllocator allocator) const
{
	frameContextBuffer_.Cleanup(allocator);
	drawDataBuffer_.Cleanup(allocator);
	transformBuffer_.Cleanup(allocator);
	parametersBuffer_.Cleanup(allocator);
}

void UInterfaceRender::UpdateBuffers(std::span<UDrawCommand const> drawCommands) const
{
	frameContextBuffer_.UpdateBuffer(
		  {}
		, MakeFrameContextAddresses()
		, {}
		, {}
		, {}
		, {}
	);

	drawDataBuffer_.UpdateMappedData<UDrawDataBufferData>(MakeDrawDataBuffer(drawCommands));
	transformBuffer_.UpdateMappedData<UTransformBufferData>(MakeTransformBuffer(drawCommands));
	parametersBuffer_.UpdateMappedData<UParametersBufferData>(MakeParametersBuffer(drawCommands));
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
		.drawDataBufferAddress = drawDataBuffer_.bda,
		.materialBufferAddress = {},
		.transformBufferAddress = transformBuffer_.bda,
		.parametersBufferAddress = parametersBuffer_.bda,

		.directionalLightBufferAddress = {},
		.pointLightBufferAddress = {},

		.clusterAABBBufferAddress = {},
		.clusterGridBufferAddress = {},
		.lightIndexBufferAddress = {},
		.lightCounterBufferAddress = {},
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
