#include "ChildrenOwnerWidget.h"
#include "log.h"

WChildrenOwnerWidget::WChildrenOwnerWidget(const WidgetVector& children) :
	children_(children)
{
}

WWidget* WChildrenOwnerWidget::GetDirty()
{
	if (isDirty_)
	{
		KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p dirty chilren owner %s", this, typeid(*this).name());
		return this;
	}

	KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p not dirty chilren owner %s", this, typeid(*this).name());
	for (auto* child : children_)
	{
		if (child)
		{
			if (auto* dirty = child->GetDirty(); dirty)
			{
				return dirty;
			}
		}
	}

	return nullptr;
}
