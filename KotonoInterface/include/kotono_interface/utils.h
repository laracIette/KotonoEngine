#pragma once
#include <kotono_framework/glm_includes.h>
glm::vec2 px_to_ndc_size(const glm::vec2 px) noexcept;
glm::vec2 ndc_to_px_size(const glm::vec2 ndc) noexcept;
glm::vec2 px_to_ndc_pos(const glm::vec2 px) noexcept;
glm::vec2 ndc_to_px_pos(const glm::vec2 ndc) noexcept; 

struct UBuildSettings
{
	glm::vec2 position;
	glm::vec2 bounds;
	int32_t layer;
};

glm::mat4 GetTranslationMatrix(const UBuildSettings& buildSettings);
glm::mat4 GetRotationMatrix(const UBuildSettings& buildSettings);
glm::mat4 GetScaleMatrix(const UBuildSettings& buildSettings);
glm::mat4 GetModelMatrix(const UBuildSettings& buildSettings);