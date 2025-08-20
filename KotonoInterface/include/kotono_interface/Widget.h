#pragma once
#include <kotono_framework/glm_includes.h>
class WWidget
{
public:
	struct BuildSettings
	{
		glm::vec2 position;
		glm::vec2 bounds;
		int layer;
	};

	struct WidgetSettings
	{
	};

	WWidget(const WidgetSettings& widgetSettings);
	virtual ~WWidget() = default;

	virtual void Build(BuildSettings buildSettings) = 0;
	virtual void Destroy();

	glm::mat4 GetTranslationMatrix(BuildSettings buildSettings) const;
	glm::mat4 GetRotationMatrix(BuildSettings buildSettings) const;
	glm::mat4 GetScaleMatrix(BuildSettings buildSettings) const;
	glm::mat4 GetModelMatrix(BuildSettings buildSettings) const;

protected:
	WidgetSettings widgetSettings_;
};

