#pragma once
#include "InterfaceObject.h"
class KInterfaceButtonComponent;
class KInterfaceTextComponent;
class RInterfaceTextButtonObject final : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	RInterfaceTextButtonObject();

private:
	KInterfaceButtonComponent* buttonComponent_;
	KInterfaceTextComponent* textComponent_;
};

