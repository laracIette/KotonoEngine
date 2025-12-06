#include "InterfaceColliderComponent.h"
#include "InterfacePhysicsManager.h"
#include <kotono_input/InputManager.h>
#include <kotono_platform/WindowViewport.h>

KInterfaceColliderComponent::KInterfaceColliderComponent(UPtrOwnerBase* ptrOwner, const UPtr<RInterfaceObject>& owner) :
	Base(ptrOwner, owner)
{
	InterfacePhysicsManager.Register(this);
}

void KInterfaceColliderComponent::Init()
{
	Base::Init();	

	InputManager.Mouse().EventButton(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed));
	InputManager.Mouse().EventButton(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED).AddListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased));
	InputManager.Mouse().EventButton(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown));
}

void KInterfaceColliderComponent::Cleanup()
{
	Base::Cleanup();

	InterfacePhysicsManager.Unregister(this);

	InputManager.Mouse().EventButton(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed));
	InputManager.Mouse().EventButton(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED).RemoveListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased));
	InputManager.Mouse().EventButton(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown));
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
