#include "InterfaceWindowObject.h"
#include "Engine.h"
#include "InterfaceTextComponent.h"

RInterfaceWindowObject::RInterfaceWindowObject(const std::string_view displayName) :
	Base(),
	displayName_(displayName)
{
	displayNameTextComponent_ = Engine.GetObjectManager().Create<KInterfaceTextComponent>(this);
}

void RInterfaceWindowObject::Init()
{
	Base::Init();

	displayNameTextComponent_->SetText(displayName_);
}

const std::string& RInterfaceWindowObject::GetDisplayName() const
{
	return displayName_;
}
