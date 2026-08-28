#pragma once
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <span>
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
	AShader(UPath const& path);

	void Init(UDevice& device, VkPipelineLayout pipelineLayout, VkFormat swapchainFormat);
	void Cleanup(UDevice& device) const;

	VkPipeline GetPipeline() const;

private:
	void CreateGraphicsPipeline(UDevice& device, VkPipelineLayout pipelineLayout, VkFormat swapchainFormat);
	void CreateComputePipeline(UDevice& device, VkPipelineLayout pipelineLayout);
	std::vector<VkFormat> GetOutputColorAttachmentFormats(EPipelinePass pipelinePass, VkFormat swapchainFormat) const;
	VkShaderModule CreateShaderModule(UDevice& device, std::span<u8 const> code);

private:
	VkPipeline pipeline_;
};
