#pragma once
#include "generated/InterfaceImageObject.generated.h"
#include "InterfaceObject.h"

class KInterfaceImageComponent;

class RInterfaceImageObject : public RInterfaceObject
{
	GENERATED_RINTERFACEIMAGEOBJECT()

public:
	RInterfaceImageObject(UPtrOwnerBase* ptrOwner);

	const UPtr<KInterfaceImageComponent>& GetImageComponent() const;

private:
	UPtr<KInterfaceImageComponent> imageComponent_;
};

