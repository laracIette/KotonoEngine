#pragma once
#include "InterfaceObject.h"

class KInterfaceTextComponent;

class RInterfaceTextBoxObject : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	void Construct() override;
	void Init() override;

	KInterfaceTextComponent* GetTextComponent() const;

private:
	KInterfaceTextComponent* textComponent_;
};

