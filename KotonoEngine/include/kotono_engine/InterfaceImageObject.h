#pragma once
#include "InterfaceObject.h"

class KInterfaceImageComponent;

class RInterfaceImageObject : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	RInterfaceImageObject();

	KInterfaceImageComponent* GetImageComponent() const;

private:
	KInterfaceImageComponent* imageComponent_;
};

