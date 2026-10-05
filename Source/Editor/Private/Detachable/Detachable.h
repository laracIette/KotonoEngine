#pragma once
#include <Widget/Widget.h>

#include "Detachable.generated.h"

class WSocket;

class WDetachable final : public WWidget
{
	GENERATED()

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