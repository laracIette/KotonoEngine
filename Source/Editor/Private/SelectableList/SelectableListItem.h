#pragma once
#include "Widget/Widget.h"

#include "SelectableListItem.generated.h"

class WButton;
class WSocket;

class WSelectableListItem final : public WWidget
{
    GENERATED()

public:
	using SelectedCallback = VoidCallback;
	
public:
	WSelectableListItem();
	
protected:
	auto Build() -> WidgetPtr override;
	
public:
	void Activate() const;
	void Deactivate() const;
	
	void SetChild(WidgetPtr const& widget) const;
	
private:
	UObjectPtr<WButton> button_;
	UObjectPtr<WSocket> socket_;
	WritableProperty(SelectedCallback, onSelected_, OnSelected);
};
