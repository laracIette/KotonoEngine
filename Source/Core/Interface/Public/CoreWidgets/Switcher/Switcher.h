#pragma once
#include <Widget/Widget.h>

#include "Switcher.generated.h"

class WStack;

class WSwitcher final : public WWidget
{
	GENERATED()
	
public:
	WSwitcher();
	
protected:
	WidgetPtr Build() override;
	
public:	
	auto GetChildren() const -> WidgetSet const&;
	void SetChildren(WidgetSet const& children) const;
	void AddChild(WidgetPtr const& child) const;
	
	void SetActiveWidget(u32 activeWidget);
	
private:
	void UpdateChildrenVisibility() const;
	
private:
	UPtr<WStack> stack_;
	ReadonlyProperty(u32, activeWidget_, ActiveWidget, Value);
};