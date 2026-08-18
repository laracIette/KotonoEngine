#pragma once
#include "DrawDataBuffer.h"
#include "FrameContextBuffer.h"
#include "ParametersBuffer.h"
#include "TransformBuffer.h"
#include <kotono_common/types.h>
#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>
struct UDrawCommand;
class UInterfaceRender final
{
public:
	void Init();
	void Cleanup() const;

	void UpdateBuffers(std::span<UDrawCommand const> drawCommands) const;
	void CmdDraw(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands) const;

private:
	UFrameContextAddresses MakeFrameContextAddresses() const;
	UFrameContextTargets MakeFrameContextTargets() const;

	std::vector<UDrawDataBufferData> MakeDrawDataBuffer(std::span<UDrawCommand const> drawCommands) const;
	std::vector<UTransformBufferData> MakeTransformBuffer(std::span<UDrawCommand const> drawCommands) const;
	std::vector<UParametersBufferData> MakeParametersBuffer(std::span<UDrawCommand const> drawCommands) const;

	void CmdDrawFrameInterface(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands) const;

	void CmdPushConstants(VkCommandBuffer commandBuffer, u32 drawIndex, u32 directionalIndex) const;

private:
	UFrameContextBuffer frameContextBuffer_;

	UDrawDataBuffer drawDataBuffer_;
	UTransformBuffer transformBuffer_;
	UParametersBuffer parametersBuffer_;
};