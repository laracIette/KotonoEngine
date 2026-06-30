#pragma once
#include <kotono_common/Path.h>
#include <span>
#include <vulkan/vulkan_core.h>
class UShader final
{
public:	
	UShader(const UPath& path);
	~UShader();

	const UPath& Path() const;

	VkPipeline GetGraphicsPipeline() const;

private:
	void CreateGraphicsPipeline();
	void CreateShaderModule(VkShaderModule& shaderModule, const std::span<u8> code);

private:
	const UPath path_;

	VkPipeline graphicsPipeline_;
};
