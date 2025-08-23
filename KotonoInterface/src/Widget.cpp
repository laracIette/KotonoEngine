#include "Widget.h"
#include "View.h"
#include "log.h"

WWidget::WWidget() :
	cachedBuild_([this]() { 
		
		return Build(); 
	})
{
}

WWidget* WWidget::Build()
{
	return this;
}

VView* WWidget::CreateView()
{
	WWidget* build{ cachedBuild_ };
	if (build && build != this)
	{
		return build->CreateView();
	}
	return new VView(this);
}

void WWidget::Destroy()
{
	auto* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		build->Destroy();
	}
	isDirty_ = false;
}

void WWidget::Rebuild()
{
	Destroy();
	auto buildSettings{ buildSettings_ };
	auto* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		buildSettings = build->buildSettings_;
	}
	CreateView()->Build(buildSettings);
}

WWidget* WWidget::GetDirty() 
{
	// todo: replace the whole hierarchy thing by a WWidget* getfirst(bool())
	if (isDirty_)
	{
		return this;
	}

	auto* build{ cachedBuild_.GetValue() };
	if (build && build != this)
	{
		return build->GetDirty();
	}
	return nullptr;
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
	isDirty_ = true;
	cachedBuild_.MarkDirty();
}
