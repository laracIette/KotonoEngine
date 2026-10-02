#pragma once
#include "generated/ObjectProperties.generated.h"
#include <kotono_core/Widget.h>
class WObjectProperties : public WWidget
{
	GENERATED_WOBJECTPROPERTIES()

public:
	WObjectProperties(ObjectPtr const& object);

protected:
	WidgetPtr Build() override;

private:
	ObjectPtr object_;
};