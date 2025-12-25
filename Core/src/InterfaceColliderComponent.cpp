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

	Mouse.EventButton(EButton::Left, EInputState::Pressed).RemoveListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed));
	Mouse.EventButton(EButton::Left, EInputState::Released).RemoveListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased));
	Mouse.EventButton(EButton::Left, EInputState::Down).RemoveListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown));
}

void KInterfaceColliderComponent::Init()
{
	Base::Init();

	Mouse.EventButton(EButton::Left, EInputState::Pressed).AddListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed));
	Mouse.EventButton(EButton::Left, EInputState::Released).AddListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased));
	Mouse.EventButton(EButton::Left, EInputState::Down).AddListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown));
}

KtEvent<>& KInterfaceColliderComponent::GetEventPressed()
{
	return eventPressed_;
}

KtEvent<>& KInterfaceColliderComponent::GetEventReleased()
{
	return eventReleased_;
}

KtEvent<>& KInterfaceColliderComponent::GetEventDown()
{
	return eventDown_;
}

KtEvent<UPtr<KInterfaceColliderComponent>>& KInterfaceColliderComponent::GetEventOverlap()
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
