#pragma once
#include <filesystem>
#include <vulkan/vulkan.h>
#include "max_frames_in_flight.h"
#include "ImageTexture.h"
#include <array>
#include "ObjectData3D.h"
#include "glm_includes.h" 
class KtShader final
{
public:
	KtShader(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath);
	~KtShader();

	VkPipeline GetGraphicsPipeline() const;
	VkPipelineLayout GetPipelineLayout() const;

	void UpdateUniformBuffer(const uint32_t imageIndex);
	void UpdateObjectBuffer(const std::vector<KtObjectData3D>& objectDatas, const uint32_t imageIndex);

	void CmdBindGraphicsPipeline(VkCommandBuffer commandBuffer);
	void CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const uint32_t imageIndex);

private:
	struct ViewProjectionBuffer
	{
		glm::mat4 View;
		glm::mat4 Projection;
	};

	const std::filesystem::path _vertPath;
	const std::filesystem::path _fragPath;

	KtImageTexture* _imageTexture;

	VkDescriptorPool _descriptorPool;
	VkDescriptorSetLayout _globalDescriptorSetLayout;
	VkDescriptorSetLayout _objectDescriptorSetLayout;
	std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _globalDescriptorSets;
	std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _objectDescriptorSets;

	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;

	std::array<VkBuffer, MAX_FRAMES_IN_FLIGHT> _uniformBuffers;
	std::array<VmaAllocation, MAX_FRAMES_IN_FLIGHT> _uniformBuffersAllocation;
	std::array<void*, MAX_FRAMES_IN_FLIGHT> _uniformBuffersMapped;

	std::array<VkBuffer, MAX_FRAMES_IN_FLIGHT> _objectBuffers;
	std::array<VmaAllocation, MAX_FRAMES_IN_FLIGHT> _objectBuffersAllocation;
	std::array<void*, MAX_FRAMES_IN_FLIGHT> _objectBuffersMapped;

	std::array<VkDeviceSize, MAX_FRAMES_IN_FLIGHT> _objectCounts;

	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();

	void CreateImageTexture();

	void CreateUniformBuffers();
	void CreateObjectBuffers();
	void CreateObjectBuffer(const uint32_t imageIndex);
	// Recreates the object buffer when the object count changes
	void SetObjectCount(const VkDeviceSize objectCount, const uint32_t imageIndex);

	void CreateDescriptorSets();
	void UpdateDescriptorSet(const uint32_t imageIndex, const KtImageTexture* imageTexture, const VkDeviceSize objectCount);

	void CreateGraphicsPipeline();
	const VkShaderModule CreateShaderModule(const std::vector<char>& code) const;
};

