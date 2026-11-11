#pragma once
#include "Widget/Widget.h"

#include "SelectableListItem.generated.h"

class WSelectableList;
class WSocket;

class WSelectableListItem final : public WWidget
{
    GENERATED()

private:
    friend WSelectableList;

public:
    using IsSelectedFunc = std::function<b8()>;
    using ClickedCallback = Callback<EModifier>;

public:
	WSelectableListItem();
    ~WSelectableListItem() override;
	
protected:
	auto Build() -> WidgetPtr override;

public:
	void SetChild(WidgetPtr const& widget) const;

private:
	UObjectPtr<WSelectableList> selectableList_;
	UObjectPtr<WSocket> socket_;

    ClickedCallback onClicked_;

    WritableProperty(IsSelectedFunc, isSelected_, IsSelected);
    WritableProperty(VoidCallback, onSelected_, OnSelected);
    WritableProperty(VoidCallback, onDeselected_, OnDeselected);
};
