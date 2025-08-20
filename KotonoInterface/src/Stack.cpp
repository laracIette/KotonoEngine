#include "Stack.h"

WStack::WStack(const StackSettings& stackSettings) :
	stackSettings_(stackSettings)
{
}

void WStack::Build(BuildSettings buildSettings)
{
	WWidget::Build(buildSettings);

	for (auto* child : stackSettings_.children)
	{
		if (child)
		{
			//++buildSettings.layer;
			child->Build(buildSettings);
		}
	}
}

void WStack::Destroy()
{
	for (auto* child : stackSettings_.children)
	{
		if (child)
		{
			child->Destroy();
		}
	}
}
