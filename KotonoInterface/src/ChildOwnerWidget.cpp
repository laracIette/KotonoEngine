#include "ChildOwnerWidget.h"
#include "log.h"

WChildOwnerWidget::WChildOwnerWidget(WWidget* child) :
	child_(child)
{
}

WWidget* WChildOwnerWidget::GetDirty()
{
	if (isDirty_)
	{
		KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p dirty child owner %s", this, typeid(*this).name());
		return this;
	}

	KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p not dirty child owner %s", this, typeid(*this).name());
	return child_->GetDirty();
}
