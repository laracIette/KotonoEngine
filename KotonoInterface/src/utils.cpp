#include "utils.h"
#include <kotono_framework/Viewport.h>

glm::vec2 px_to_ndc_size(const glm::vec2 px) noexcept
{
    return px / glm::vec2(WindowViewport.GetExtent()) * 2.0f;
}

glm::vec2 ndc_to_px_size(const glm::vec2 ndc) noexcept
{
    return ndc * glm::vec2(WindowViewport.GetExtent()) / 2.0f;
}

glm::vec2 px_to_ndc_pos(const glm::vec2 px) noexcept
{
    return px / glm::vec2(WindowViewport.GetExtent()) * 2.0f - 1.0f;
}

glm::vec2 ndc_to_px_pos(const glm::vec2 ndc) noexcept
{
    return (ndc + 1.0f) * glm::vec2(WindowViewport.GetExtent()) / 2.0f;
}

glm::mat4 GetTranslationMatrix(const UBuildSettings& buildSettings)
{
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(buildSettings.position), 0.0f });
}

glm::mat4 GetRotationMatrix(const UBuildSettings& buildSettings)
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, { 0.0f, 0.0f, 1.0f });
}

glm::mat4 GetScaleMatrix(const UBuildSettings& buildSettings)
{
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(buildSettings.bounds), 1.0f });
}

glm::mat4 GetModelMatrix(const UBuildSettings& buildSettings)
{
	return GetTranslationMatrix(buildSettings) * GetRotationMatrix(buildSettings) * GetScaleMatrix(buildSettings);
}