#include "Stack.h"

WStack::WStack(const StackSettings& stackSettings) :
	stackSettings_(stackSettings)
{
}

void WStack::Build(BuildSettings buildSettings)
{
	for (auto* child : stackSettings_.children)
	{
		if (child)
		{
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
