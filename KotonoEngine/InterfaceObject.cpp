#include "InterfaceObject.h"
#include "log.h"
#include <kotono_framework/Viewport.h>
#include "Engine.h"
#include "InterfaceComponent.h"
#include "ObjectManager.h"

void RInterfaceObject::Construct()
{
	Base::Construct();

	viewport_ = &WindowViewport;
	rootComponent_ = AddComponent<KInterfaceComponent>();
}

void RInterfaceObject::Init()
{
	Base::Init();

	visibility_ = EVisibility::EditorAndGame;
}

void RInterfaceObject::Update()
{
	Base::Update();
}

void RInterfaceObject::Cleanup()
{
	Base::Cleanup();
}

KtViewport* RInterfaceObject::GetViewport() const
{
	return viewport_;
}

RInterfaceObject* RInterfaceObject::GetParent() const
{
	return parent_;
}

KInterfaceComponent* RInterfaceObject::GetRootComponent() const
{
	return rootComponent_;
}

const int32_t RInterfaceObject::GetLayer() const
{
	if (parent_)
	{
		return layer_ + parent_->GetLayer() + 1;
	}
	return layer_;
}

const std::unordered_set<RInterfaceObject*>& RInterfaceObject::GetChildren() const
{
	return children_;
}

const EVisibility RInterfaceObject::GetVisibility() const
{
	return visibility_;
}

void RInterfaceObject::SetVisibility(const EVisibility visibility)
{
	visibility_ = visibility;
}

void RInterfaceObject::SetViewport(KtViewport* viewport)
{
	viewport_ = viewport;
}

void RInterfaceObject::SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect)
{
	if (parent == this)
	{
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "RInterfaceObject::SetParent(): couldn't set the parent of '%s' to itself", GetName().c_str());
		return;
	}
	if (parent == parent_)
	{
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "RInterfaceObject::SetParent(): couldn't set the parent of '%s' to the same", GetName().c_str());
		return;
	}
	if (parent)
	{
		parent->children_.insert(this);
	}
	if (parent_)
	{
		parent_->children_.erase(this);
	}
	parent_ = parent;
	GetRootComponent()->SetParent(parent_ ? parent_->GetRootComponent() : nullptr, keepRect);
}

void RInterfaceObject::SetLayer(const int32_t layer)
{
	layer_ = layer;
}

void RInterfaceObject::AddComponent(KInterfaceComponent* component)
{
	components_.insert(component);
}