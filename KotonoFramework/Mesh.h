#pragma once
#include "Model.h"
#include "Shader.h"
#include "max_frames_in_flight.h"
class KtMesh final
{
public:
	void AddToRenderQueue(const glm::mat4& modelMatrix) const;

	KtShader* GetShader() const;
	KtModel* GetModel() const;

	void SetShader(KtShader* shader);
	void SetModel(KtModel* model);

private:
	KtShader* _shader;
	KtModel* _model;

	std::array<VkBuffer, MAX_FRAMES_IN_FLIGHT> _uniformBuffers;
	std::array<VmaAllocation, MAX_FRAMES_IN_FLIGHT> _uniformBuffersAllocation;
	std::array<void*, MAX_FRAMES_IN_FLIGHT> _uniformBuffersMapped;
	VkDescriptorPool _descriptorPool;
	std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _uniformDescriptorSets;
};

