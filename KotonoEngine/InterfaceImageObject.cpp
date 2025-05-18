#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"

void RInterfaceImageObject::Construct()
{
	Base::Construct();

	image_ = AddComponent<KInterfaceImageComponent>();
}
