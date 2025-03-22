#pragma once
#include <filesystem>
#include <array>
#include <vulkan/vulkan.h>
#include "frames_in_flight.h"
#include "Shader.h"
#include "ImageTexture.h"
#include "UniformData2D.h"
#include "ObjectData2D.h"
#include "glm_includes.h" 
#include "AllocatedBuffer.h"
class KtShader2D final : public KtShader
{
public:
	KtShader2D();

	void Init() override;
	void Cleanup() override;

	VkPipeline GetGraphicsPipeline() const;
	VkPipelineLayout GetPipelineLayout() const;

	void UpdateUniformBuffer(const KtUniformData2D& uniformData, const uint32_t imageIndex);
	void UpdateObjectBuffer(const std::vector<KtObjectData2D>& objectDatas, const uint32_t imageIndex);

	void CmdBind(VkCommandBuffer commandBuffer) const;
	void CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const uint32_t imageIndex);

private:
	const std::filesystem::path _vertPath;
	const std::filesystem::path _fragPath;

    KtImageTexture _imageTexture;

	VkDescriptorPool _descriptorPool;
	VkDescriptorSetLayout _uniformDescriptorSetLayout;
	VkDescriptorSetLayout _objectDescriptorSetLayout;
	std::array<VkDescriptorSet, KT_FRAMES_IN_FLIGHT> _uniformDescriptorSets;
	std::array<VkDescriptorSet, KT_FRAMES_IN_FLIGHT> _objectDescriptorSets;

	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;

	std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT> _uniformBuffers;
	std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT> _objectBuffers;
	std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT> _stagingUniformBuffers;
	std::array<KtAllocatedBuffer, KT_FRAMES_IN_FLIGHT> _stagingObjectBuffers;
	std::array<VkDeviceSize, KT_FRAMES_IN_FLIGHT> _objectCounts;

	void CreateImageTexture();

	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void UpdateDescriptorSet(const uint32_t imageIndex);

	void CreateUniformBuffers();
	void CreateObjectBuffers();
	void CreateUniformBuffer(const uint32_t imageIndex);
	void CreateObjectBuffer(const uint32_t imageIndex);
	// Recreates the object buffer when the object count changes
	void SetObjectCount(const VkDeviceSize objectCount, const uint32_t imageIndex);
	const VkDeviceSize GetObjectBufferSize(const uint32_t imageIndex) const;

	void CreateGraphicsPipeline();
	const VkShaderModule CreateShaderModule(const std::vector<char>& code) const;
};

