#include "InterfaceObjectColliderComponent.h"
#include "Engine.h"
#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/Viewport.h>
#include <kotono_framework/Collection.h>

void KInterfaceObjectColliderComponent::Init()
{
	Base::Init();	

	ListenEvent(Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED), &KInterfaceObjectColliderComponent::OnEventMouseLeftButtonPressed);
	ListenEvent(Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED), &KInterfaceObjectColliderComponent::OnEventMouseLeftButtonReleased);
	ListenEvent(Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN), &KInterfaceObjectColliderComponent::OnEventMouseLeftButtonDown);
}

void KInterfaceObjectColliderComponent::Update()
{
	Base::Update();

	UpdateOverlaps();
	BroadcastOverlaps();
}

void KInterfaceObjectColliderComponent::Cleanup()
{
	Base::Cleanup();
}

KtEvent<>& KInterfaceObjectColliderComponent::GetEventPressed()
{
	return _eventPressed;
}

KtEvent<>& KInterfaceObjectColliderComponent::GetEventReleased()
{
	return _eventReleased;
}

KtEvent<>& KInterfaceObjectColliderComponent::GetEventDown()
{
	return _eventDown;
}

KtEvent<KInterfaceObjectColliderComponent*>& KInterfaceObjectColliderComponent::GetEventOverlap()
{
	return _eventOverlap;
}

const bool KInterfaceObjectColliderComponent::GetIsMouseOverlapping() const
{
	const auto& cursorPosition = Framework.GetInputManager().GetMouse().GetCursorPosition();
	const auto viewportSize = glm::vec2(WindowViewport.GetExtent());
	const auto worldPosition = cursorPosition / viewportSize * 2.0f - glm::vec2(1.0f);
	return GetRect().GetIsOverlapping(worldPosition);
}

void KInterfaceObjectColliderComponent::UpdateOverlaps()
{
	auto overlaps = KtCollection(Engine.GetObjectManager().GetAllOfType<KInterfaceObjectColliderComponent>());
	overlaps.AddFilter([this](auto* collider) { return collider != this; });
	overlaps.AddFilter([this](auto* collider) { return GetRect().GetIsOverlapping(collider->GetRect()); });
	_overlaps = overlaps.GetUnorderedSet();
}

void KInterfaceObjectColliderComponent::BroadcastOverlaps()
{
	for (auto* interfaceCollider : _overlaps)
	{
		_eventOverlap.Broadcast(interfaceCollider);
	}
}

void KInterfaceObjectColliderComponent::OnEventMouseLeftButtonPressed()
{
	if (!GetIsMouseOverlapping())
	{
		return;
	}

	for (const auto* interfaceCollider : _overlaps)
	{
		if (interfaceCollider->GetVisibility() != EVisibility::None &&
			interfaceCollider->GetLayer() > GetLayer() && 
			interfaceCollider->GetIsMouseOverlapping())
		{
			return;
		}
	}

	_isPressed = true;
	_eventPressed.Broadcast();
}

void KInterfaceObjectColliderComponent::OnEventMouseLeftButtonReleased()
{
	if (!_isPressed)
	{
		return;
	}

	_isPressed = false;
	_eventReleased.Broadcast();
}

void KInterfaceObjectColliderComponent::OnEventMouseLeftButtonDown()
{
	if (!_isPressed)
	{
		return;
	}

	_eventDown.Broadcast();
}
