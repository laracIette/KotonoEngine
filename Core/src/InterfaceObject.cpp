#include "InterfaceObject.h"
#include "log.h"
#include <kotono_platform/WindowViewport.h>
#include "InterfaceComponent.h"
#include "ObjectManager.h"

RInterfaceObject::RInterfaceObject(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	viewport_ = &WindowViewport;
	rootComponent_ = ObjectManager.Create<KInterfaceComponent>(Ptr<RInterfaceObject>());
	AddComponent(rootComponent_);
}

void RInterfaceObject::Init()
{
	Base::Init();
}

void RInterfaceObject::Cleanup()
{
	Base::Cleanup();

	for (const auto& component : components_)
	{
		component->Delete();
	}
}

KtWindowViewport* RInterfaceObject::GetViewport() const
{
	return viewport_;
}

const UPtr<RInterfaceObject>& RInterfaceObject::GetParent() const
{
	return parent_;
}

const UPtr<KInterfaceComponent>& RInterfaceObject::RootComponent() const
{
	return rootComponent_;
}

const KtPool<UPtr<RInterfaceObject>>& RInterfaceObject::GetChildren() const
{
	return children_;
}

void RInterfaceObject::SetViewport(KtWindowViewport* viewport)
{
	viewport_ = viewport;
}

void RInterfaceObject::SetParent(const UPtr<RInterfaceObject>& parent, const ECoordinateSpace keepRect)
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
		const size_t index{ childrenIndex_ };
		if (parent_->children_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
		{
			parent_->children_[index]->childrenIndex_ = index;
		}
	}
	if (parent)
	{
		parent->children_.Add(Ptr<RInterfaceObject>());
		childrenIndex_ = parent->children_.LastIndex();
	}
	parent_ = parent;
	RootComponent()->SetParent(parent_ ? parent_->RootComponent() : nullptr, keepRect);
}

bool RInterfaceObject::IsHovered() const
{
	return std::any_of(components_.begin(), components_.end(),
		[](const UPtr<KInterfaceComponent>& component) { return component->IsHovered(); }
	);
}

void RInterfaceObject::AddComponent(const UPtr<KInterfaceComponent>& component)
{
	components_.Add(component);
	component->componentIndex_ = components_.LastIndex();
}

void RInterfaceObject::RemoveComponent(const UPtr<KInterfaceComponent>& component)
{
	const size_t index{ component->componentIndex_ };
	if (components_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
	{
		components_[index]->componentIndex_ = index;
	}
}
