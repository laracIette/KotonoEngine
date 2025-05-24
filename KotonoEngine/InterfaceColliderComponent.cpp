#include "InterfaceColliderComponent.h"
#include "Engine.h"
#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/Viewport.h>
#include <kotono_framework/Collection.h>

void KInterfaceColliderComponent::Init()
{
	Base::Init();	

	ListenEvent(Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED), &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed);
	ListenEvent(Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED), &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased);
	ListenEvent(Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN), &KInterfaceColliderComponent::OnEventMouseLeftButtonDown);
}

void KInterfaceColliderComponent::Update()
{
	Base::Update();

	UpdateOverlaps();
	BroadcastOverlaps();
}

void KInterfaceColliderComponent::Cleanup()
{
	Base::Cleanup();
}

KtEvent<>& KInterfaceColliderComponent::GetEventPressed()
{
	return _eventPressed;
}

KtEvent<>& KInterfaceColliderComponent::GetEventReleased()
{
	return _eventReleased;
}

KtEvent<>& KInterfaceColliderComponent::GetEventDown()
{
	return _eventDown;
}

KtEvent<KInterfaceColliderComponent*>& KInterfaceColliderComponent::GetEventOverlap()
{
	return _eventOverlap;
}

const bool KInterfaceColliderComponent::GetIsMouseOverlapping() const
{
	const auto& cursorPosition = Framework.GetInputManager().GetMouse().GetCursorPosition();
	const auto viewportSize = glm::vec2(WindowViewport.GetExtent());
	const auto worldPosition = cursorPosition / viewportSize * 2.0f - glm::vec2(1.0f);
	return GetRect().GetIsOverlapping(worldPosition);
}

void KInterfaceColliderComponent::UpdateOverlaps()
{
	auto overlaps = KtCollection(Engine.GetObjectManager().GetAll<KInterfaceColliderComponent>());
	overlaps.AddFilter([this](auto* collider) { return collider != this; });
	overlaps.AddFilter([this](auto* collider) { return GetRect().GetIsOverlapping(collider->GetRect()); });
	_overlaps = overlaps.GetUnorderedSet();
}

void KInterfaceColliderComponent::BroadcastOverlaps()
{
	for (auto* interfaceCollider : _overlaps)
	{
		_eventOverlap.Broadcast(interfaceCollider);
	}
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonPressed()
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

	KT_DEBUG_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "collider: %s pressed", GetName().c_str());
	_isPressed = true;
	_eventPressed.Broadcast();
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonReleased()
{
	if (!_isPressed)
	{
		return;
	}

	_isPressed = false;
	_eventReleased.Broadcast();
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonDown()
{
	if (!_isPressed)
	{
		return;
	}

	_eventDown.Broadcast();
}
