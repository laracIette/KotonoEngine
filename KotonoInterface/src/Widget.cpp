#include "Widget.h"
#include "utils.h"

void WWidget::Build(BuildSettings buildSettings)
{
	position_ = buildSettings.position;
	size_ = buildSettings.bounds;
}

glm::vec2 WWidget::GetPosition() const
{
	return position_;
}

glm::vec2 WWidget::GetSize() const
{
	return size_;
}

glm::mat4 WWidget::GetTranslationMatrix() const
{
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(position_), 0.0f });
}

glm::mat4 WWidget::GetRotationMatrix() const
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, { 0.0f, 0.0f, 1.0f });
}

glm::mat4 WWidget::GetScaleMatrix() const
{
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(size_), 1.0f });
}

glm::mat4 WWidget::GetModelMatrix() const
{
	return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}