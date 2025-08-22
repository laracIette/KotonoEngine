#include "StackView.h"
#include "Stack.h"

VStackView::VStackView(WStack* stack) :
	VView(stack),
	stack_(stack)
{
}

void VStackView::Build(UBuildSettings buildSettings)
{
	if (!stack_)
	{
		return;
	}

	VView::Build(buildSettings);

	for (auto* child : stack_->stackSettings_.children)
	{
		if (child)
		{
			++buildSettings.layer;
			child->CreateView()->Build(buildSettings);
		}
	}
}
