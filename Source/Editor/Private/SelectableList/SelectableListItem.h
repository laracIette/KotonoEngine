#pragma once
#include "Widget/Widget.h"

#include "SelectableListItem.generated.h"

class KButtonStyle_Class;
class WSelectableList;
class WSocket;

class WSelectableListItem final : public WWidget
{
    GENERATED()

private:
    friend WSelectableList;

public:
    using ClickedCallback = Callback<EModifier>;

public:
	WSelectableListItem();
    ~WSelectableListItem() override;
	
protected:
	auto Build() -> WidgetPtr override;

public:
	void SetChild(WidgetPtr const& widget) const;

private:
	UObjectPtr<WSocket> socket_;

    ClickedCallback onClicked_;
    f32 lastClickTime_;

    WritableProperty(UPtr<KButtonStyle_Class const>, buttonStyleClass_, ButtonStyleClass);
    WritableProperty(UBindable<b8>, isSelected_, IsSelected);
    WritableProperty(VoidCallback, onSelected_, OnSelected);
    WritableProperty(VoidCallback, onDeselected_, OnDeselected);
    WritableProperty(VoidCallback, onDoubleClicked_, OnDoubleClicked);
    WritableProperty(EAxis, wrapAxis_, WrapAxis);
};
