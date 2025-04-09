#include "InterfaceObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/log.h>
#include "Engine.h"
#include "ObjectManager.h"

void RInterfaceObject::Init()
{
	_visibility = EVisibility::EditorAndGame;
	_viewport = &WindowViewport;
	_eventOverlap.AddListener(this, &RInterfaceObject::OnEventOverlap);
	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.AddListener(this, &RInterfaceObject::OnEventMouseLeftButtonDown);
}

void RInterfaceObject::Update()
{
	UpdateOverlaps();
	BroadcastOverlaps();
}

void RInterfaceObject::Cleanup()
{
	_eventOverlap.RemoveListener(this);
	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.RemoveListener(this);
}

void RInterfaceObject::UpdateOverlaps()
{
	auto interfaceObjects = Engine.GetObjectManager().GetAllOfType<RInterfaceObject>();
	interfaceObjects.erase(this);

	_overlaps.clear();

	for (auto* interfaceObject : interfaceObjects)
	{
		if (_rect.GetIsOverlapping(interfaceObject->_rect))
		{
			_overlaps.insert(interfaceObject);
		}
	}
}

void RInterfaceObject::BroadcastOverlaps()
{
	for (auto* interfaceObject : _overlaps)
	{
		_eventOverlap.Broadcast(interfaceObject);
	}
}

const URect& RInterfaceObject::GetRect() const
{
	return _rect;
}

URect& RInterfaceObject::GetRect() 
{
	return _rect;
}

KtViewport* RInterfaceObject::GetViewport() const
{
	return _viewport;
}

RInterfaceObject* RInterfaceObject::GetParent() const
{
	return _parent;
}

KtEvent<RInterfaceObject*>& RInterfaceObject::GetEventOverlap()
{
	return _eventOverlap;
}

const EVisibility RInterfaceObject::GetVisibility() const
{
	return _visibility;
}

void RInterfaceObject::SetVisibility(const EVisibility visibility)
{
	_visibility = visibility;
}

void RInterfaceObject::SetViewport(KtViewport* viewport)
{
	_viewport = viewport;
}

void RInterfaceObject::SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect)
{
	if (parent == this)
	{
		KT_DEBUG_LOG("RInterfaceObject::SetParent(): couldn't set the parent of '%s' to itself", GetName().c_str());
		return;
	}
	if (parent == _parent)
	{
		KT_DEBUG_LOG("RInterfaceObject::SetParent(): couldn't set the parent of '%s' to the same", GetName().c_str());
		return;
	}
	_parent = parent;
	_rect.SetParent(_parent ? &_parent->_rect : nullptr, keepRect);
}

void RInterfaceObject::OnEventOverlap(RInterfaceObject* other)
{
	KT_DEBUG_LOG("'%s' overlapping '%s'", other->GetName().c_str(), GetName().c_str());
}

void RInterfaceObject::OnEventMouseLeftButtonDown()
{
	const auto& cursorPosition = Framework.GetInputManager().GetMouse().GetCursorPosition();
	if (_rect.GetIsOverlapping(cursorPosition))
	{
		const auto& windowSize = Framework.GetWindow().GetSize();
		const auto cursorPositionDelta = Framework.GetInputManager().GetMouse().GetCursorPositionDelta();
		const auto cursorPositionDeltaNormalized = 2.0f * cursorPositionDelta / glm::vec2(windowSize);
		_rect.AddOffset(cursorPositionDeltaNormalized);
		KT_DEBUG_LOG("added offset: %s to '%s'", glm::to_string(cursorPositionDeltaNormalized).c_str(), GetName().c_str());
	}
}
