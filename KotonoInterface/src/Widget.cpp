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
	return GetBuild()->CreateView();
}

void WWidget::Destroy()
{
}

WWidget* WWidget::GetDirty()
{
	if (isDirty_)
	{
		KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p dirty widget %s", this, typeid(*this).name());
		return this;
	}

	KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p not dirty widget %s", this, typeid(*this).name());

	auto* build = GetBuild();
	return build == this ? nullptr : build->GetDirty();
}

glm::vec2 WWidget::GetPosition() const
{
	return position_;
}

glm::vec2 WWidget::GetSize() const
{
	return size_;
}

void WWidget::SetState(const StateFunction& function)
{
	function();
	isDirty_ = true;
	cachedBuild_.MarkDirty();
}

WWidget* WWidget::GetBuild()
{
	return cachedBuild_;
}
