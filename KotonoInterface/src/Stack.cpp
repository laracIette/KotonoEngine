#include "Stack.h"
#include "StackView.h"

WStack::WStack(const StackSettings& stackSettings) :
	WChildrenOwnerWidget(stackSettings.children),
	stackSettings_(stackSettings)
{
}

VView* WStack::CreateView()
{
	return new VStackView(this);
}