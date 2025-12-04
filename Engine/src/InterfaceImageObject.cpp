#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"
#include "Engine.h"
#include "ObjectManager.h"

RInterfaceImageObject::RInterfaceImageObject(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	imageComponent_ = Engine.ObjectManager().Create<KInterfaceImageComponent>(Ptr<RInterfaceImageObject>());
	AddComponent(imageComponent_);
}

const UPtr<KInterfaceImageComponent>& RInterfaceImageObject::GetImageComponent() const
{
	return imageComponent_;
}
