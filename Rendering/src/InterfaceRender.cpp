#include "InterfaceRender.h"

#include "DrawCommand.h"
#include "DrawDataBufferData.h"
#include "IndexBuffer.h"
#include "ParametersBufferData.h"
#include "PipelineResourceManager.h"
#include "PushConstants.h"
#include "TransformBufferData.h"
#include <kotono_platform/Device.h>
#include <ranges>
#include <vector>

static constexpr u32 MAX_DRAW_DATAS{ 65536 };

static constexpr b8 equal(VkRect2D const& left, VkRect2D const& right) noexcept
{
	return left.offset.x == right.offset.x
		&& left.offset.y == right.offset.y
		&& left.extent.width == right.extent.width
		&& left.extent.height == right.extent.height;
}

UInterfaceRender::UInterfaceRender(UDevice& device, UPipelineResourceManager& pipelineResourceManager)
	: device_{ device }
	, pipelineResourceManager_{ pipelineResourceManager }
	, frameContextBuffer_{ device }
{
}

void UInterfaceRender::Init()
{
	frameContextBuffer_.Init();

	drawDataBuffer_ = device_.CreateAllocatedBuffer(
		  sizeof(UDrawDataBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
	transformBuffer_ = device_.CreateAllocatedBuffer(
		  sizeof(UTransformBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
	parametersBuffer_ = device_.CreateAllocatedBuffer(
		  sizeof(UParametersBufferData) * MAX_DRAW_DATAS
		, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
		| VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);
}

void UInterfaceRender::Cleanup() const
{
	frameContextBuffer_.Cleanup();
	device_.CleanupAllocatedBuffer(drawDataBuffer_);
	device_.CleanupAllocatedBuffer(transformBuffer_);
	device_.CleanupAllocatedBuffer(parametersBuffer_);
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
	, std::span<UDrawCommand const> drawCommands
	, UIndexBuffer const& indexBuffer
) const
{
	CmdDrawFrameInterface(commandBuffer, drawCommands, indexBuffer);
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
	, std::span<UDrawCommand const> drawCommands
	, UIndexBuffer const& indexBuffer
) const
{
	indexBuffer.CmdBind(commandBuffer);
		
	for (size i{ 0 }; i < drawCommands.size();)
	{
		auto const& drawCommand{ drawCommands[i] };

		u32 instances{ 1 };
		while (i + instances < drawCommands.size())
		{
			if (drawCommands[i + instances].pipeline != drawCommand.pipeline
			 || !equal(drawCommands[i + instances].scissor, drawCommand.scissor))
			{
				break;
			}
			++instances;
		}

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, drawCommand.pipeline);

		CmdPushConstants(commandBuffer, drawCommand.drawIndex, 0);

		vkCmdSetScissor(commandBuffer, 0, 1, &drawCommand.scissor);

		vkCmdDrawIndexed(commandBuffer
			, drawCommand.indexCount
			, instances
			, drawCommand.firstIndex
			, 0
			, 0
		);

		i += instances;
	}
}

void UInterfaceRender::CmdPushConstants(
	  VkCommandBuffer commandBuffer
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
		, pipelineResourceManager_.GetPipelineLayout()
		, VK_SHADER_STAGE_ALL
		, 0
		, sizeof(UPushConstants)
		, &pc
	);
}
