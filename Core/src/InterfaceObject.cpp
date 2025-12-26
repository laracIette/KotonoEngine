#include "InterfaceObject.h"
#include <kotono_common/log.h>
#include <kotono_platform/WindowViewport.h>
#include "InterfaceComponent.h"
#include "ObjectManager.h"

RInterfaceObject::RInterfaceObject(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	viewport_ = &WindowViewport;
	rootComponent_ = ObjectManager.Create<KInterfaceComponent>();
	rootComponent_->SetOwner(Ptr());
}

void RInterfaceObject::Init()
{
}

void RInterfaceObject::Cleanup()
{
	Base::Cleanup();

	for (const auto& component : interfaceComponents_)
	{
		component->Delete();
	}
}

KtWindowViewport* RInterfaceObject::GetViewport() const
{
	return viewport_;
}

UPtr<RInterfaceObject>& RInterfaceObject::GetParent() 
{
	return parent_;
}

UPtr<KInterfaceComponent>& RInterfaceObject::RootComponent() 
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

void RInterfaceObject::SetParent(UPtr<RInterfaceObject> parent, const ECoordinateSpace keepRect)
{
	if (parent == this)
	{
		KT_LOG(ELogImportanceLevel::High, "Core.RInterfaceObject::SetParent()", "couldn't set the parent of '%s' to itself", GetName().c_str());
		return;
	}
	if (parent == parent_)
	{
		KT_LOG(ELogImportanceLevel::High, "Core.RInterfaceObject::SetParent()", "couldn't set the parent of '%s' to the same", GetName().c_str());
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
		parent->children_.Add(Ptr());
		childrenIndex_ = parent->children_.LastIndex();
	}
	parent_ = parent;
	RootComponent()->SetParent(parent_ ? parent_->RootComponent() : nullptr, keepRect);
}

bool RInterfaceObject::IsHovered() const
{
	return std::any_of(interfaceComponents_.begin(), interfaceComponents_.end(),
		[](const UPtr<KInterfaceComponent>& component) { return component->IsHovered(); }
	);
}

void RInterfaceObject::AddComponent(UPtr<KInterfaceComponent> component)
{
	interfaceComponents_.Add(component);
	component->componentIndex_ = interfaceComponents_.LastIndex();
}

void RInterfaceObject::RemoveComponent(const UPtr<KInterfaceComponent>& component)
{
	const size_t index{ component->componentIndex_ };
	if (interfaceComponents_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
	{
		interfaceComponents_[index]->componentIndex_ = index;
	}
}
