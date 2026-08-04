#pragma once
#include <glm/vec2.hpp>
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
class SWindowViewport final
{
public:
	static const glm::uvec2& GetExtent();
	static const glm::ivec2& GetOffset();

	static bool GetIsKeepAspectRatio();
	static f32 GetAspectRatio();

	static void SetExtent(const glm::uvec2& extent);
	static void SetOffset(const glm::ivec2& offset);

	static void SetIsKeepAspectRatio(const bool isKeepAspectRatio);
	static void SetAspectRatio(const f32 aspectRatio);

	static void CmdUse(VkCommandBuffer commandBuffer);

private:
	static glm::uvec2 extent_;
	static glm::ivec2 offset_;

	static bool isKeepAspectRatio_;
	static f32 aspectRatio_;
};
