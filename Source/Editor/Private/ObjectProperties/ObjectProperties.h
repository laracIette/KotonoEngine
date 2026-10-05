#pragma once
#include "ObjectProperties.generated.h"
#include <Widget/Widget.h>

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