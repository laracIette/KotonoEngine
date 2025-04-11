#include "InterfaceObject.h"
#include <kotono_framework/log.h>

void RInterfaceObject::Init()
{
	Base::Init();
	_visibility = EVisibility::EditorAndGame;
	_viewport = &WindowViewport;
}

void RInterfaceObject::Update()
{
	Base::Update();
}

void RInterfaceObject::Cleanup()
{
	Base::Cleanup();
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

const int32_t RInterfaceObject::GetLayer() const
{
	if (_parent)
	{
		return _layer + _parent->GetLayer() + 1;
	}
	return _layer;
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

void RInterfaceObject::SetLayer(const int32_t layer)
{
	_layer = layer;
}

