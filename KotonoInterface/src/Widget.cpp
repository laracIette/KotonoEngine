#include "Widget.h"
#include "log.h"

WWidget::WWidget() :
	cachedBuild_([this]() { return Build(); })
{
}

void WWidget::CacheBuild()
{
	cachedBuild_.TryUpdateValue();
	WWidget* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		build->CacheBuild();
	}
}

WWidget* WWidget::Build()
{
	return this;
}

void WWidget::Cleanup()
{
	WWidget* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		build->Cleanup();
		delete build;
		cachedBuild_ = nullptr;
	}
}

void WWidget::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);

	WWidget* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		build->Display(displaySettings);
	}
	else
	{
		displaySettings = GetDisplaySettings(displaySettings);
		DisplayInternal(displaySettings);
	}
}

WWidget::DisplaySettings WWidget::GetDisplaySettings(DisplaySettings displaySettings) const
{
	const WWidget* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		return build->GetDisplaySettings(displaySettings);
	}

	return displaySettings;
}

EFlex WWidget::GetFlex() const
{
	const WWidget* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		return build->GetFlex();
	}
	return EFlex::None;
}

WWidget::WidgetVector WWidget::GetWidgetTree()
{
	WWidget* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		return { build };
	}
	return { this };
}

void WWidget::Rebuild()
{
	auto displaySettings{ displaySettings_ };
	const WWidget* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		displaySettings = build->displaySettings_;
	}
	Cleanup();
	CacheBuild();
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

void WWidget::SetParent(WWidget* parent)
{
	parent_ = parent;
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

glm::mat4 WWidget::TranslationMatrix() const
{
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(displaySettings_.position + displaySettings_.bounds / 2.0f), 0.0f });
}

glm::mat4 WWidget::RotationMatrix() const
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, { 0.0f, 0.0f, 1.0f });
}

glm::mat4 WWidget::ScaleMatrix() const
{
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(displaySettings_.bounds), 1.0f });
}

glm::mat4 WWidget::ModelMatrix() const
{
	return TranslationMatrix() * RotationMatrix() * ScaleMatrix();
}

void WWidget::DisplayInternal(DisplaySettings displaySettings)
{
}
