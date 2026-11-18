#pragma once

#include "AllocatedBuffer.h"
#include "Containers/Span.h"
#include "FrameContextBuffer/FrameContextBuffer.h"
#include "types.h"

#include <vector>
#include <vulkan/vulkan_core.h>

struct UDrawCommand;
struct UDrawDataBufferData;
struct UTransformBufferData;
struct UParametersBufferData;
class UDevice;
class UIndexBuffer;
class UPipelineResourceManager;

class UInterfaceRender final
{
public:
	explicit UInterfaceRender(UDevice& device, UPipelineResourceManager& pipelineResourceManager);

	void Init();
	void Cleanup() const;

	void UpdateBuffers(USpan<UDrawCommand const> drawCommands) const;
	void CmdDraw(VkCommandBuffer commandBuffer, USpan<UDrawCommand const> drawCommands, UIndexBuffer const& indexBuffer) const;

private:
	auto MakeFrameContextAddresses() const -> UFrameContextAddresses;

	auto MakeDrawDataBuffer(USpan<UDrawCommand const> drawCommands) const -> std::vector<UDrawDataBufferData>;
	auto MakeTransformBuffer(USpan<UDrawCommand const> drawCommands) const -> std::vector<UTransformBufferData>;
	auto MakeParametersBuffer(USpan<UDrawCommand const> drawCommands) const -> std::vector<UParametersBufferData>;

	void CmdDrawFrameInterface(VkCommandBuffer commandBuffer, USpan<UDrawCommand const> drawCommands, UIndexBuffer const& indexBuffer) const;

	void CmdPushConstants(VkCommandBuffer commandBuffer, u32 drawIndex, u32 directionalIndex) const;

private:
	UDevice& device_;
	UPipelineResourceManager& pipelineResourceManager_;

	UFrameContextBuffer frameContextBuffer_;

	UAllocatedBuffer drawDataBuffer_;
	UAllocatedBuffer transformBuffer_;
	UAllocatedBuffer parametersBuffer_;
};