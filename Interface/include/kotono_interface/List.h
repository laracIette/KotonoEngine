#pragma once
#include "generated/List.generated.h"
#include <kotono_core/Widget.h>
class WListBody;
/// Defines a vertical container for widgets
class WList final : public WWidget
{
	GENERATED_WLIST()

protected:
	WidgetPtr Build() override;

public:
	f32 GetSpacing() const;
	WidgetSet const& GetChildren() const;

	void SetSpacing(f32 spacing);
	void SetChildren(WidgetSet const& children);

private:
	UPtr<WListBody> body_;
};

