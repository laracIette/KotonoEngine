#include "InterfaceTextButtonObject.h"
#include "InterfaceButtonComponent.h"
#include "InterfaceTextComponent.h"

RInterfaceTextButtonObject::RInterfaceTextButtonObject() :
	Base()
{
	buttonComponent_ = AddComponent<KInterfaceButtonComponent>();
	textComponent_ = AddComponent<KInterfaceTextComponent>();
}
