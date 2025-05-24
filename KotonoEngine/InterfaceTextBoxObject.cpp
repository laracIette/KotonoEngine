#include "InterfaceTextBoxObject.h"
#include "InterfaceTextComponent.h"
#include "InterfaceColliderComponent.h"

void RInterfaceTextBoxObject::Construct()
{
	Base::Construct();

	textComponent_ = AddComponent<KInterfaceTextComponent>();
	colliderComponent_ = AddComponent<KInterfaceColliderComponent>();
}

void RInterfaceTextBoxObject::Init()
{
	Base::Init();
}

KInterfaceTextComponent* RInterfaceTextBoxObject::GetTextComponent() const
{
	return textComponent_;
}
