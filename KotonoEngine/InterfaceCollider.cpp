#include "InterfaceCollider.h"
#include "Engine.h"
#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/log.h>

void RInterfaceCollider::Init()
{
	Base::Init();	

	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.AddListener(this, &RInterfaceCollider::OnEventMouseLeftButtonPressed);
	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED)
		.AddListener(this, &RInterfaceCollider::OnEventMouseLeftButtonReleased);
	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.AddListener(this, &RInterfaceCollider::OnEventMouseLeftButtonDown);
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

	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.RemoveListener(this);
	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED)
		.RemoveListener(this);
	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.RemoveListener(this);
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
	return GetRect().GetIsOverlapping(cursorPosition);
}

void RInterfaceCollider::UpdateOverlaps()
{
	auto interfaceColliders = Engine.GetObjectManager().GetAllOfType<RInterfaceCollider>();
	interfaceColliders.erase(this);

	_overlaps.clear();

	for (auto* interfaceCollider : interfaceColliders)
	{
		if (GetRect().GetIsOverlapping(interfaceCollider->GetRect()))
		{
			_overlaps.insert(interfaceCollider);
		}
	}
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
