#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"

void RInterfaceImageObject::Construct()
{
	Base::Construct();

	imageComponent_ = AddComponent<KInterfaceImageComponent>();
}

KInterfaceImageComponent* RInterfaceImageObject::GetImageComponent() const
{
	return imageComponent_;
}
