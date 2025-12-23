#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"
#include "ObjectManager.h"

RInterfaceImageObject::RInterfaceImageObject(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	imageComponent_ = ObjectManager.Create<KInterfaceImageComponent>();
	imageComponent_->SetOwner(Ptr());
}

UPtr<KInterfaceImageComponent>& RInterfaceImageObject::ImageComponent()
{
	return imageComponent_;
}
