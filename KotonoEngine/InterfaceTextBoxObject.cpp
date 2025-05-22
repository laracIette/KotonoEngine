#include "InterfaceTextBoxObject.h"
#include "InterfaceTextComponent.h"

void RInterfaceTextBoxObject::Construct()
{
	Base::Construct();

	textComponent_ = AddComponent<KInterfaceTextComponent>();
}

void RInterfaceTextBoxObject::Init()
{
	Base::Init();
}

KInterfaceTextComponent* RInterfaceTextBoxObject::GetTextComponent() const
{
	return textComponent_;
}
