#pragma once
#include "AddToRenderQueue3DArgs.h"
#include "UniformData3D.h"
#include "Culler3D.h"
class KtRenderer3D final
{
public:
	void Init();
	void Cleanup();

	void AddToRenderQueue(const KtAddToRenderQueue3DArgs& args);
	void SetUniformData(const KtUniformData3D& uniformData);

	void CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const;
	
	void Reset(const uint32_t currentFrame);

private:
	std::array<KtRenderQueue3DData, KT_FRAMES_IN_FLIGHT> _renderQueueData;
	std::array<KtUniformData3D, KT_FRAMES_IN_FLIGHT> _uniformData;
	KtCuller3D _culler;
};

