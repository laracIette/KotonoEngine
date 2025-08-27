#include "StackView.h"
#include "Stack.h"

VStackView::VStackView(WStack* stack) :
	VView(stack)
{
}

void VStackView::Build(UBuildSettings buildSettings)
{
	if (!widget_)
	{
		return;
	}

	VView::Build(buildSettings);

	auto* asStack{ static_cast<WStack*>(widget_) };

	for (auto* child : asStack->stackSettings_.children)
	{
		if (child)
		{
			++buildSettings.layer;
			child->CreateView()->Build(buildSettings);
		}
	}
}
