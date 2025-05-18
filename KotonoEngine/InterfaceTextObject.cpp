#include "InterfaceTextObject.h"
#include "InterfaceTextComponent.h"

void RInterfaceTextObject::Construct()
{
	Base::Construct();

	text_ = AddComponent<KInterfaceTextComponent>();
}
