#include "InterfaceWindowComponent.h"
#include "Engine.h"
#include "InterfaceObject.h"
#include "InterfaceTextComponent.h"
#include "InterfaceBoxComponent.h"

KInterfaceWindowComponent::KInterfaceWindowComponent(RInterfaceObject* owner) :
	Base(owner),
	displayName_("Testetste")
{
	displayNameTextComponent_ = GetOwner()->AddComponent<KInterfaceTextComponent>();
	backgroundComponent_ = GetOwner()->AddComponent<KInterfaceBoxComponent>();
}

void KInterfaceWindowComponent::Init()
{
	Base::Init();

	displayNameTextComponent_->SetText(displayName_);
	backgroundComponent_->SetColor(KtColor::FromHSV({ .v = 50.0f }, 0.5f));
	backgroundComponent_->SetScreenSize({ 256.0f, 256.0f });

	displayNameTextComponent_->SetLayer(1);
	backgroundComponent_->SetLayer(0);
}

const std::string& KInterfaceWindowComponent::GetDisplayName() const
{
	return displayName_;
}