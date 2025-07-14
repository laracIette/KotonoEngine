#include "InterfaceTextObject.h"
#include "InterfaceTextComponent.h"

RInterfaceTextObject::RInterfaceTextObject() :
	Base()
{
	text_ = AddComponent<KInterfaceTextComponent>();
}
