#include "InterfaceColliderComponent.h"
#include "InterfacePhysicsManager.h"
#include <kotono_input/Mouse.h>
#include <kotono_platform/WindowViewport.h>

KInterfaceColliderComponent::KInterfaceColliderComponent(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	InterfacePhysicsManager.Register(this);
}

void KInterfaceColliderComponent::Cleanup()
{
	Base::Cleanup();

	InterfacePhysicsManager.Unregister(this);

	Mouse.EventButton(EButton::Left, EInputState::Pressed).RemoveListener(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed);
	Mouse.EventButton(EButton::Left, EInputState::Released).RemoveListener(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased);
	Mouse.EventButton(EButton::Left, EInputState::Down).RemoveListener(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown);
}

void KInterfaceColliderComponent::Init()
{
	Base::Init();

	Mouse.EventButton(EButton::Left, EInputState::Pressed).AddListener(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed);
	Mouse.EventButton(EButton::Left, EInputState::Released).AddListener(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased);
	Mouse.EventButton(EButton::Left, EInputState::Down).AddListener(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown);
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
