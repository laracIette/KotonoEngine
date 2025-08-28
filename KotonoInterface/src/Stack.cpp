#include "Stack.h"

WStack::WStack(const StackSettings& stackSettings) :
	WChildrenOwnerWidget(stackSettings.children),
	stackSettings_(stackSettings)
{
}

void WStack::Display(BuildSettings buildSettings)
{
	SetBuildSettings(buildSettings);

	for (auto* child : stackSettings_.children)
	{
		if (child)
		{
			++buildSettings.layer;
			child->Display(buildSettings);
		}
	}
}