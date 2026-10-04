#pragma once
#include "generated/Detachable.generated.h"
#include <kotono_core/Widget.h>
class WSocket;
class WDetachable final : public WWidget
{
	GENERATED_WDETACHABLE()

public:
	WDetachable();
	
protected:
	WidgetPtr Build() override;

public:
	auto GetChild() const -> WidgetPtr;
	void SetChild(WidgetPtr const& widget) const;

private:
	void Detach();

private:
	UPtr<WSocket> socket_;
};