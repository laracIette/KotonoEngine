#pragma once
#include "InterfaceObject.h"
class KInterfaceButtonComponent;
class KInterfaceTextComponent;
class RInterfaceTextButtonObject final : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	void Construct() override;

private:
	KInterfaceButtonComponent* buttonComponent_;
	KInterfaceTextComponent* textComponent_;
};

