#include "InterfaceColliderComponent.h"
#include "Engine.h"
#include "InterfacePhysicsManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/Viewport.h>

KInterfaceColliderComponent::KInterfaceColliderComponent(RInterfaceObject* owner) :
	Base(owner)
{
	Engine.GetInterfacePhysicsManager().Register(this);
}

void KInterfaceColliderComponent::Init()
{
	Base::Init();	

	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED).AddListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN).AddListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown));
}

void KInterfaceColliderComponent::Update()
{
	Base::Update();
}

void KInterfaceColliderComponent::Cleanup()
{
	Base::Cleanup();

	Engine.GetInterfacePhysicsManager().Unregister(this);

	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED).RemoveListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown));
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

KtEvent<KInterfaceColliderComponent*>& KInterfaceColliderComponent::GetEventOverlap()
{
	return eventOverlap_;
}

const bool KInterfaceColliderComponent::GetIsMouseOverlapping() const
{
	const auto& cursorPosition = Framework.GetInputManager().GetMouse().GetCursorPosition();
	const auto viewportSize = glm::vec2(WindowViewport.GetExtent());
	const auto worldPosition = cursorPosition / viewportSize * 2.0f - glm::vec2(1.0f);
	return GetIsOverlapping(worldPosition);
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonPressed()
{
	if (!GetIsMouseOverlapping())
	{
		return;
	}

	for (const auto* interfaceCollider : overlaps_)
	{
		if (interfaceCollider->GetVisibility() != EVisibility::None &&
			interfaceCollider->GetLayer() > GetLayer() && 
			interfaceCollider->GetIsMouseOverlapping())
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

	if (GetIsMouseOverlapping())
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
