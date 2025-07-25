#pragma once
#include "InterfaceObject.h"

class KInterfaceTextComponent;

class RInterfaceTextObject : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	RInterfaceTextObject();

private:
	KInterfaceTextComponent* text_;
};

