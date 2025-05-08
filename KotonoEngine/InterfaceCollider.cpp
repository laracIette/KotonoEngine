#include "InterfaceCollider.h"
#include "Engine.h"
#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/log.h>
#include <kotono_framework/Collection.h>

void RInterfaceCollider::Init()
{
	Base::Init();	

	ListenEvent(Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED), &RInterfaceCollider::OnEventMouseLeftButtonPressed);
	ListenEvent(Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED), &RInterfaceCollider::OnEventMouseLeftButtonReleased);
	ListenEvent(Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN), &RInterfaceCollider::OnEventMouseLeftButtonDown);
}

void RInterfaceCollider::Update()
{
	Base::Update();
	UpdateOverlaps();
	BroadcastOverlaps();
}

void RInterfaceCollider::Cleanup()
{
	Base::Cleanup();
}

KtEvent<>& RInterfaceCollider::GetEventPressed()
{
	return _eventPressed;
}

KtEvent<>& RInterfaceCollider::GetEventReleased()
{
	return _eventReleased;
}

KtEvent<>& RInterfaceCollider::GetEventDown()
{
	return _eventDown;
}

KtEvent<RInterfaceCollider*>& RInterfaceCollider::GetEventOverlap()
{
	return _eventOverlap;
}

const bool RInterfaceCollider::GetIsMouseOverlapping() const
{
	const auto& cursorPosition = Framework.GetInputManager().GetMouse().GetCursorPosition();
	const auto viewportSize = glm::vec2(WindowViewport.GetExtent());
	const auto worldPosition = cursorPosition / viewportSize * 2.0f - glm::vec2(1.0f);
	return GetRect().GetIsOverlapping(worldPosition);
}

void RInterfaceCollider::UpdateOverlaps()
{
	auto overlaps = KtCollection(Engine.GetObjectManager().GetAllOfType<RInterfaceCollider>());
	overlaps.AddFilter([this](auto* collider) { return collider != this; });
	overlaps.AddFilter([this](auto* collider) { return GetRect().GetIsOverlapping(collider->GetRect()); });
	_overlaps = overlaps.GetUnorderedSet();
}

void RInterfaceCollider::BroadcastOverlaps()
{
	for (auto* interfaceCollider : _overlaps)
	{
		_eventOverlap.Broadcast(interfaceCollider);
	}
}

void RInterfaceCollider::OnEventMouseLeftButtonPressed()
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

void RInterfaceCollider::OnEventMouseLeftButtonReleased()
{
	if (!_isPressed)
	{
		return;
	}

	_isPressed = false;
	_eventReleased.Broadcast();
}

void RInterfaceCollider::OnEventMouseLeftButtonDown()
{
	if (!_isPressed)
	{
		return;
	}

	_eventDown.Broadcast();
}
