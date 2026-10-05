#pragma once
#include <Widget/Widget.h>

#include "ObjectProperties.generated.h"

class WObjectProperties : public WWidget
{
	GENERATED()

public:
	WObjectProperties(ObjectPtr const& object);

protected:
	WidgetPtr Build() override;

private:
	ObjectPtr object_;
};