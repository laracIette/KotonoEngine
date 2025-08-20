#pragma once
#include "InterfaceObject.h"
class KInterfaceRowComponent;
class RInterfaceLayoutObject : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	RInterfaceLayoutObject();

protected:
	void Init() override;

private:
	KInterfaceRowComponent* windowsArrayComponent_;
};

