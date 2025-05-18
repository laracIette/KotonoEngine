#pragma once
#include "InterfaceObject.h"

class KInterfaceTextComponent;

class RInterfaceTextObject : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	void Construct() override;

private:
	KInterfaceTextComponent* text_;
};

