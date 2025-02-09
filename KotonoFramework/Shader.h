#pragma once
#include <filesystem>
#include <vulkan/vulkan.h>
class KtShader final
{
public:
	KtShader(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath);
	~KtShader();

	const VkPipeline GetGraphicsPipeline() const;

	void Bind() const;

private:
	const std::filesystem::path _vertPath;
	const std::filesystem::path _fragPath;

	VkPipeline _graphicsPipeline;

	void CreateGraphicsPipeline();
	const VkShaderModule CreateShaderModule(const std::vector<char>& code) const;
};

