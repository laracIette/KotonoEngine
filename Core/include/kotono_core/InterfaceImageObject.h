#pragma once
#include "generated/InterfaceImageObject.generated.h"
#include "InterfaceObject.h"

class KInterfaceImageComponent;

class RInterfaceImageObject : public RInterfaceObject
{
	GENERATED_RINTERFACEIMAGEOBJECT()

public:
	RInterfaceImageObject(UPtrOwnerBase* ptrOwner);

	UPtr<KInterfaceImageComponent>& ImageComponent();

private:
	UPtr<KInterfaceImageComponent> imageComponent_;
};

