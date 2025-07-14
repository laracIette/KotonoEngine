#include "InterfaceTextBoxObject.h"
#include "InterfaceTextComponent.h"
#include "InterfaceColliderComponent.h"
#include "Timer.h"

RInterfaceTextBoxObject::RInterfaceTextBoxObject() :
	Base()
{
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

void RInterfaceTextBoxObject::Cleanup()
{
	Base::Cleanup();

	colliderComponent_->GetEventPressed().RemoveListener(KtDelegate<>(this, &RInterfaceTextBoxObject::OnMousePressed));
	colliderComponent_->GetEventReleased().RemoveListener(KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseReleased));
	colliderComponent_->GetEventDown().RemoveListener(KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseDown));
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
		colliderComponent_->GetEventPressed().AddListener(KtDelegate<>(this, &RInterfaceTextBoxObject::OnMousePressed));
		colliderComponent_->GetEventReleased().AddListener(KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseReleased));
		colliderComponent_->GetEventDown().AddListener(KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseDown));
	}
	else
	{
		colliderComponent_->GetEventPressed().RemoveListener(KtDelegate<>(this, &RInterfaceTextBoxObject::OnMousePressed));
		colliderComponent_->GetEventReleased().RemoveListener(KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseReleased));
		colliderComponent_->GetEventDown().RemoveListener(KtDelegate<>(this, &RInterfaceTextBoxObject::OnMouseDown));
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
