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
	
	colliderComponent_->GetRect().SetParent(&textComponent_->GetRect(), ECoordinateSpace::Relative);
	colliderComponent_->GetRect().SetRelativeSize(glm::vec2(0.5f, 0.2f));
}

void RInterfaceTextBoxObject::Update()
{
	Base::Update();
}

const bool RInterfaceTextBoxObject::GetIsEditable() const
{
	return isEditable_;
}

void RInterfaceTextBoxObject::SetIsEditable(const bool isEditable)
{
	if (isEditable_ == isEditable)
	{
		return;
	}

	isEditable_ = isEditable;

	if (isEditable_)
	{
		ListenEvent(colliderComponent_->GetEventPressed(), &RInterfaceTextBoxObject::OnMousePressed);
		ListenEvent(colliderComponent_->GetEventReleased (), &RInterfaceTextBoxObject::OnMouseReleased);
		ListenEvent(colliderComponent_->GetEventDown(), &RInterfaceTextBoxObject::OnMouseDown);
	}
	else
	{
		UnlistenEvent(colliderComponent_->GetEventPressed(), &RInterfaceTextBoxObject::OnMousePressed);
		UnlistenEvent(colliderComponent_->GetEventReleased(), &RInterfaceTextBoxObject::OnMouseReleased);
		UnlistenEvent(colliderComponent_->GetEventDown(), &RInterfaceTextBoxObject::OnMouseDown);
	}
}

KInterfaceTextComponent* RInterfaceTextBoxObject::GetTextComponent() const
{
	return textComponent_;
}

void RInterfaceTextBoxObject::OnMousePressed()
{
}

void RInterfaceTextBoxObject::OnMouseReleased()
{
}

void RInterfaceTextBoxObject::OnMouseDown()
{
	
}
