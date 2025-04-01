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
	
	void Reset();

private:
	KtRenderQueue3DData _renderQueueData;
	KtUniformData3D _uniformData;
	KtCuller3D _culler;
};

