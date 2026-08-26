#pragma once
#include "FrameContextBuffer.h"
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>
struct UDrawCommand;
struct UDrawDataBufferData;
struct UTransformBufferData;
struct UParametersBufferData;
class UIndexBuffer;
class UInterfaceRender final
{
public:
	void Init(VkDevice device, VmaAllocator allocator);
	void Cleanup(VmaAllocator allocator) const;

	void UpdateBuffers(std::span<UDrawCommand const> drawCommands) const;
	void CmdDraw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, std::span<UDrawCommand const> drawCommands, UIndexBuffer const& indexBuffer) const;

private:
	UFrameContextAddresses MakeFrameContextAddresses() const;

	std::vector<UDrawDataBufferData> MakeDrawDataBuffer(std::span<UDrawCommand const> drawCommands) const;
	std::vector<UTransformBufferData> MakeTransformBuffer(std::span<UDrawCommand const> drawCommands) const;
	std::vector<UParametersBufferData> MakeParametersBuffer(std::span<UDrawCommand const> drawCommands) const;

	void CmdDrawFrameInterface(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, std::span<UDrawCommand const> drawCommands, UIndexBuffer const& indexBuffer) const;

	void CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, u32 drawIndex, u32 directionalIndex) const;

private:
	UFrameContextBuffer frameContextBuffer_;

	UAllocatedBuffer drawDataBuffer_;
	UAllocatedBuffer transformBuffer_;
	UAllocatedBuffer parametersBuffer_;
};