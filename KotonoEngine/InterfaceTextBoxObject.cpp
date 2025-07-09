#include "InterfaceTextBoxObject.h"
#include "InterfaceTextComponent.h"
#include "InterfaceColliderComponent.h"
#include "Timer.h"

void RInterfaceTextBoxObject::Construct()
{
	Base::Construct();

	textComponent_ = AddComponent<KInterfaceTextComponent>();
	colliderComponent_ = AddComponent<KInterfaceColliderComponent>();
}

void RInterfaceTextBoxObject::Init()
{
	Base::Init();
	
	colliderComponent_->SetParent(textComponent_, ECoordinateSpace::Relative);
	colliderComponent_->SetRelativeSize(glm::vec2(0.5f, 0.2f));
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
		ListenEvent(colliderComponent_->GetEventPressed(), 
			KtDelegate<>(this, &RInterfaceTextBoxObject::OnMousePressed));
		ListenEvent(colliderComponent_->GetEventReleased(), 
			KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseReleased));
		ListenEvent(colliderComponent_->GetEventDown(), 
			KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseDown));
	}
	else
	{
		UnlistenEvent(colliderComponent_->GetEventPressed(), 
			KtDelegate<>(this, &RInterfaceTextBoxObject::OnMousePressed));
		UnlistenEvent(colliderComponent_->GetEventReleased(), 
			KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseReleased));
		UnlistenEvent(colliderComponent_->GetEventDown(), 
			KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseDown));
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
