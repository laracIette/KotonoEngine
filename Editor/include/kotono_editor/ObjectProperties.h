#pragma once
#include "generated/ObjectProperties.generated.h"
#include <kotono_object/Widget.h>
class WObjectProperties : public WWidget
{
	GENERATED_WOBJECTPROPERTIES()

public:
	WObjectProperties(const UPtr<KObject>& object);

protected:
	WidgetPtr Build() override;

private:
	WidgetPtr BuildMemberWidget(const std::string& type, void* variablePtr);

private:
	UPtr<KObject> object_;
};