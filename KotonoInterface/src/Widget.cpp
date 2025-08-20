#include "Widget.h"
#include "utils.h"

glm::mat4 WWidget::GetTranslationMatrix(BuildSettings buildSettings) const
{
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(buildSettings.position), 0.0f });
}

glm::mat4 WWidget::GetRotationMatrix(BuildSettings buildSettings) const
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, { 0.0f, 0.0f, 1.0f });
}

glm::mat4 WWidget::GetScaleMatrix(BuildSettings buildSettings) const
{
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(buildSettings.bounds), 1.0f });
}

glm::mat4 WWidget::GetModelMatrix(BuildSettings buildSettings) const
{
	return GetTranslationMatrix(buildSettings) * GetRotationMatrix(buildSettings) * GetScaleMatrix(buildSettings);
}