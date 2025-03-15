#include "Renderer2D.h"
#include <log.h>

constexpr std::array<float, 8> vertices = 
{   //  x      y
	-0.5f, -0.5f, // Bottom-left
	 0.5f, -0.5f, // Bottom-right
	 0.5f,  0.5f, // Top-right
	-0.5f,  0.5f  // Top-left
};
constexpr std::array<uint32_t, 6> indices = { 0, 1, 2, 2, 3, 0 };

void KtRenderer2D::CmdDraw(VkCommandBuffer commandBuffer, const uint32_t currentFrame)
{
	for (auto& renderQueue2DData : _renderer2DData.RenderQueues)
	{
		for (auto& [shader, shaderData] : renderQueue2DData.Shaders)
		{
			if (!shader)
			{
				KT_DEBUG_LOG("shader is nullptr");
				continue;
			}

			shader->UpdateUniformBuffer(_uniformData2D, currentFrame);
			shader->UpdateObjectBuffer(shaderData.ObjectDatas, currentFrame);

			shader->CmdBind(commandBuffer);
			shader->CmdBindDescriptorSets(commandBuffer, currentFrame);

			VkBuffer vertexBuffers[] = { _vertexBuffer.Buffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffer, _indexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);

			const uint32_t instanceCount = static_cast<uint32_t>(shaderData.ObjectDatas.size());
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), instanceCount, 0, 0, 0);
		}
	}
}
