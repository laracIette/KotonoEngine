#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"

RInterfaceImageObject::RInterfaceImageObject() :
	Base()
{
	imageComponent_ = AddComponent<KInterfaceImageComponent>();
}

KInterfaceImageComponent* RInterfaceImageObject::GetImageComponent() const
{
	return imageComponent_;
}
