#pragma once
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
class UShader final
{
public:	
	UShader(const UPath& path);
	~UShader();

	const UPath& Path() const;

	VkPipeline GetPipeline() const;

private:
	void CreateGraphicsPipeline();
	void CreateComputePipeline();
	std::vector<VkFormat> GetOutputColorAttachmentFormats(const EPipelinePass pipelinePass) const;
	void CreateShaderModule(VkShaderModule& shaderModule, const std::span<u8> code);

private:
	const UPath path_;

	VkPipeline pipeline_;
};
