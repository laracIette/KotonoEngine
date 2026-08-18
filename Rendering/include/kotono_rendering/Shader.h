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
	AShader(UPath const& path, VkFormat swapChainFormat);
	~AShader() override;

	VkPipeline GetPipeline() const;

private:
	void CreateGraphicsPipeline(VkFormat swapChainFormat);
	void CreateComputePipeline();
	std::vector<VkFormat> GetOutputColorAttachmentFormats(EPipelinePass pipelinePass, VkFormat swapChainFormat) const;
	void CreateShaderModule(VkShaderModule& shaderModule, std::span<u8 const> code);

private:
	VkPipeline pipeline_;
};
