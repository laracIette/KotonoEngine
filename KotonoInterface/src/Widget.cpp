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

void WWidget::Display(BuildSettings buildSettings)
{
	SetBuildSettings(buildSettings);

	WWidget* build{ cachedBuild_ };
	if (build && build != this)
	{
		build->Display(buildSettings);
	}
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
	auto buildSettings{ buildSettings_ };
	WWidget* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		buildSettings = build->buildSettings_;
	}
	Cleanup();
	Display(buildSettings);
}

glm::vec2 WWidget::GetPosition() const
{
	return buildSettings_.position;
}

glm::vec2 WWidget::GetSize() const
{
	return buildSettings_.bounds;
}

void WWidget::SetState(const StateFunction& function)
{
	function();
	cachedBuild_.MarkDirty();
	Rebuild();
}

void WWidget::SetBuildSettings(const BuildSettings& buildSettings)
{
	buildSettings_ = buildSettings;
}

glm::mat4 WWidget::GetTranslationMatrix() const
{
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(buildSettings_.position), 0.0f });
}

glm::mat4 WWidget::GetRotationMatrix() const
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, { 0.0f, 0.0f, 1.0f });
}

glm::mat4 WWidget::GetScaleMatrix() const
{
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(buildSettings_.bounds), 1.0f });
}

glm::mat4 WWidget::GetModelMatrix() const
{
	return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}