#include "InterfaceLayoutObject.h"
#include "InterfaceRowComponent.h"
#include "InterfaceWindowComponent.h"

RInterfaceLayoutObject::RInterfaceLayoutObject() :
	Base()
{
	windowsArrayComponent_ = AddComponent<KInterfaceRowComponent>();
}

void RInterfaceLayoutObject::Init()
{
	Base::Init();

	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
	windowsArrayComponent_->AddItem(AddComponent<KInterfaceWindowComponent>());
}
