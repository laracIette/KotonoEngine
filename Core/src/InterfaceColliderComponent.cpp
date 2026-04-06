#include "InterfaceColliderComponent.h"
#include "InterfacePhysicsManager.h"
#include <kotono_input/Mouse.h>
#include <kotono_platform/WindowViewport.h>

KInterfaceColliderComponent::KInterfaceColliderComponent(UPtrOwner* ptrOwner) 
	: Base(ptrOwner)
	, isPressed_(false)
{
}

KInterfaceColliderComponent::~KInterfaceColliderComponent()
{
	InterfacePhysicsManager.Unregister(this);
}

void KInterfaceColliderComponent::Init()
{
	Base::Init();

	InterfacePhysicsManager.Register(this);

	RegisterDelegate(&Mouse, Mouse.EventButton(EButton::Left, EInputState::Pressed), this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed);
	RegisterDelegate(&Mouse, Mouse.EventButton(EButton::Left, EInputState::Released), this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased);
	RegisterDelegate(&Mouse, Mouse.EventButton(EButton::Left, EInputState::Down), this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown);
}

UEvent<>& KInterfaceColliderComponent::GetEventPressed()
{
	return eventPressed_;
}

UEvent<>& KInterfaceColliderComponent::GetEventReleased()
{
	return eventReleased_;
}

UEvent<>& KInterfaceColliderComponent::GetEventDown()
{
	return eventDown_;
}

UEvent<UPtr<KInterfaceColliderComponent>>& KInterfaceColliderComponent::GetEventOverlap()
{
	return eventOverlap_;
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonPressed()
{
	if (!IsHovered())
	{
		return;
	}

	for (const auto& interfaceCollider : overlaps_)
	{
		if (interfaceCollider->GetVisibility() == EVisibility::Visible &&
			interfaceCollider->GetLayer() > GetLayer() && 
			interfaceCollider->IsHovered())
		{
			return;
		}
	}

	isPressed_ = true;
	eventPressed_.Broadcast();
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonReleased()
{
	if (!isPressed_)
	{
		return;
	}

	isPressed_ = false;

	if (IsHovered())
	{
		eventReleased_.Broadcast();
	}
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonDown()
{
	if (!isPressed_)
	{
		return;
	}

	eventDown_.Broadcast();
}

#include "generated/InterfaceColliderComponent.generated.inl"
