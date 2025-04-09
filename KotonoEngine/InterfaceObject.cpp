#include "InterfaceObject.h"
#include <kotono_framework/log.h>
#include "Engine.h"
#include "ObjectManager.h"

void RInterfaceObject::Init()
{
	_visibility = EVisibility::EditorAndGame;
	_viewport = &WindowViewport;
}

void RInterfaceObject::Update()
{
	UpdateOverlaps();
	for (auto* interfaceObject : _overlaps)
	{
		interfaceObject->_eventOverlap.Broadcast();
	}
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

KtEvent& RInterfaceObject::GetEventOverlap()
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
