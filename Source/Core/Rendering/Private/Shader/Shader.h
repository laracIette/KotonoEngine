#pragma once

#include "Asset/Asset.h"

#include "Containers/Span.h"

#include <vector>
#include <vulkan/vulkan_core.h>

class UDevice;

enum class EPipelinePass : u8
{
	Compute,
	ShadowPrePass,
	DepthPrePass,
	GBuffer,
	DeferredLighting,
	PostProcess,
	Interface,
};

class AShader final : public AAsset
{
public:	
	explicit AShader(UPath const& path);

	void Init(UDevice& device, VkPipelineLayout pipelineLayout, VkFormat swapchainFormat);
	void Cleanup(UDevice& device) const;

	auto GetPipeline() const -> VkPipeline;

private:
	void CreateGraphicsPipeline(UDevice& device, VkPipelineLayout pipelineLayout, VkFormat swapchainFormat);
	void CreateComputePipeline(UDevice& device, VkPipelineLayout pipelineLayout);
	auto GetOutputColorAttachmentFormats(EPipelinePass pipelinePass, VkFormat swapchainFormat) const -> std::vector<VkFormat>;
	auto CreateShaderModule(UDevice& device, USpan<u8 const> code) -> VkShaderModule;

private:
	VkPipeline pipeline_;
};
