#pragma once
#include "generated/List.generated.h"
#include "Widget.h"
class WListBody;
/// Defines a vertical container for widgets
class WList final : public WWidget
{
	GENERATED_WLIST()

	friend class WListBody;

protected:
	WidgetPtr Build() override;

public:
	float GetSpacing() const;
	const WidgetPool& GetChildren() const;

	void SetSpacing(const float spacing);
	void SetChildren(const WidgetPool& children);

private:
	UPtr<WListBody> body_;
};

