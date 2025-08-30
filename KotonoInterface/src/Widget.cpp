#include "Widget.h"
#include "log.h"

WWidget::WWidget() :
	cachedBuild_([this]() { return Build(); })
{
}

WWidget* WWidget::Build()
{
	return this;
}

void WWidget::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);

	WWidget* build{ cachedBuild_ };
	if (build && build != this)
	{
		build->Display(displaySettings);
	}
}

WWidget::DisplaySettings WWidget::GetDisplaySettings(DisplaySettings displaySettings) const
{
	displaySettings.bounds = { 0.0f, 0.0f };
	return displaySettings;
}

void WWidget::Cleanup()
{
	WWidget* build{ cachedBuild_.GetValue() };
	if (build != this)
	{
		if (build)
		{
			build->Cleanup();
		}
		delete build;
	}
}

void WWidget::Rebuild()
{
	auto displaySettings{ displaySettings_ };
	WWidget* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		displaySettings = build->displaySettings_;
	}
	Cleanup();
	Display(displaySettings);
}

glm::vec2 WWidget::GetPosition() const
{
	return displaySettings_.position;
}

glm::vec2 WWidget::GetSize() const
{
	return displaySettings_.bounds;
}

void WWidget::SetState(const StateFunction& function)
{
	function();
	cachedBuild_.MarkDirty();
	Rebuild();
}

void WWidget::SetDisplaySettings(const DisplaySettings& displaySettings)
{
	displaySettings_ = displaySettings;
}

glm::mat4 WWidget::GetTranslationMatrix() const
{
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(displaySettings_.position + displaySettings_.bounds / 2.0f), 0.0f });
}

glm::mat4 WWidget::GetRotationMatrix() const
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, { 0.0f, 0.0f, 1.0f });
}

glm::mat4 WWidget::GetScaleMatrix() const
{
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(displaySettings_.bounds), 1.0f });
}

glm::mat4 WWidget::GetModelMatrix() const
{
	return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}