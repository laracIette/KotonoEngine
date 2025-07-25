#include "InterfaceObject.h"
#include "log.h"
#include <kotono_framework/Viewport.h>
#include "Engine.h"
#include "InterfaceComponent.h"
#include "ObjectManager.h"
#include <kotono_framework/Stopwatch.h>

RInterfaceObject::RInterfaceObject() :
	Base()
{
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

	for (auto* component : components_)
	{
		component->Delete();
	}
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

const KtPool<RInterfaceObject*>& RInterfaceObject::GetChildren() const
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
	if (parent_)
	{
		const size_t index = childrenIndex_;
		parent_->children_.RemoveAt(index);
		if (index < parent_->children_.Size())
		{
			parent_->children_[index]->childrenIndex_ = index;
		}
	}
	if (parent)
	{
		parent->children_.Add(this);
		childrenIndex_ = parent->children_.LastIndex();
	}
	parent_ = parent;
	GetRootComponent()->SetParent(parent_ ? parent_->GetRootComponent() : nullptr, keepRect);
}

void RInterfaceObject::AddComponent(KInterfaceComponent* component)
{
	Engine.GetObjectManager().Register(component);
	components_.Add(component);
	component->componentIndex_ = components_.LastIndex();
}

void RInterfaceObject::RemoveComponent(const KInterfaceComponent* component)
{
	const size_t index = component->componentIndex_;
	components_.RemoveAt(index);
	if (index < components_.Size())
	{
		components_[index]->componentIndex_ = index;
	}
}
