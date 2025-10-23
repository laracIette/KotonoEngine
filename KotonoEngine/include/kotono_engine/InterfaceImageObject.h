#pragma once
#include "InterfaceObject.h"

class KInterfaceImageComponent;

class RInterfaceImageObject : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	RInterfaceImageObject(UPtrOwnerBase* ptrOwner);

	const UPtr<KInterfaceImageComponent>& GetImageComponent() const;

private:
	UPtr<KInterfaceImageComponent> imageComponent_;
};

