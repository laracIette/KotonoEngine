#include "InterfaceTextButtonObject.h"
#include "InterfaceButtonComponent.h"
#include "InterfaceTextComponent.h"

void RInterfaceTextButtonObject::Construct()
{
	Base::Construct();

	buttonComponent_ = AddComponent<KInterfaceButtonComponent>();
	textComponent_ = AddComponent<KInterfaceTextComponent>();
}
