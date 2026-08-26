#pragma once
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <span>
#include <vulkan/vulkan_core.h>
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

	void Init(VkFormat swapChainFormat, VkPipelineLayout pipelineLayout);
	void Cleanup(VkDevice device) const;

	VkPipeline GetPipeline() const;

private:
	void CreateGraphicsPipeline(VkFormat swapChainFormat, VkPipelineLayout pipelineLayout);
	void CreateComputePipeline(VkPipelineLayout pipelineLayout);
	std::vector<VkFormat> GetOutputColorAttachmentFormats(EPipelinePass pipelinePass, VkFormat swapChainFormat) const;
	VkShaderModule CreateShaderModule(std::span<u8 const> code);

private:
	VkPipeline pipeline_;
};
