#include "InterfaceObject.h"
#include "InterfaceComponent.h"
#include <kotono_common/log.h>
#include <kotono_platform/WindowViewport.h>

RInterfaceObject::RInterfaceObject(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	viewport_ = &WindowViewport;
}

void RInterfaceObject::Cleanup()
{
	for (int64_t i{ interfaceComponents_.LastIndex() }; i >= 0 && i < interfaceComponents_.size(); --i)
	{
		interfaceComponents_[i]->Delete();
	}

	SetParent(nullptr, ECoordinateSpace::Relative);

	Base::Cleanup();
}

void RInterfaceObject::Init()
{
}

void RInterfaceObject::Update(const float deltaTime)
{
}

bool RInterfaceObject::GetCanUpdate() const
{
	return canUpdate_;
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

void RInterfaceObject::SetCanUpdate(const bool canUpdate)
{
	canUpdate_ = canUpdate;
}

void RInterfaceObject::SetViewport(KtWindowViewport* viewport)
{
	viewport_ = viewport;
}

void RInterfaceObject::SetParent(const UPtr<RInterfaceObject>& parent, const ECoordinateSpace keepRect)
{
	if (!parent && !parent_)
	{
		return;
	}

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

	parent_ = parent;

	if (parent_)
	{
		parent_->children_.Add(Ptr());
		childrenIndex_ = parent_->children_.LastIndex();
	}

	if (rootComponent_)
	{
		rootComponent_->SetParent(parent_ ? parent_->RootComponent() : nullptr, keepRect);
	}
}

bool RInterfaceObject::IsHovered() const
{
	return std::any_of(interfaceComponents_.begin(), interfaceComponents_.end(),
		[](const UPtr<KInterfaceComponent>& component) { return component->IsHovered(); }
	);
}

void RInterfaceObject::AddComponent(const UPtr<KInterfaceComponent>& component)
{
	if (interfaceComponents_.empty())
	{
		rootComponent_ = component;
	}
	else
	{
		component->SetParent(rootComponent_, ECoordinateSpace::Relative);
	}
	interfaceComponents_.Add(component);
	component->componentIndex_ = interfaceComponents_.LastIndex();
}

void RInterfaceObject::RemoveComponent(const UPtr<KInterfaceComponent>& component)
{
	const size_t index{ component->componentIndex_ };
	if (interfaceComponents_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
	{
		interfaceComponents_[index]->componentIndex_ = index;
		if (component == rootComponent_)
		{
			rootComponent_ = interfaceComponents_[index];
		}
	}
	else if (component == rootComponent_)
	{
		rootComponent_ = nullptr;
	}

	for (size_t i{ 1 }; i < interfaceComponents_.size(); ++i)
	{
		interfaceComponents_[i]->SetParent(rootComponent_, ECoordinateSpace::Relative);
	}
}

void RInterfaceObject::Spawn()
{
	for (auto& interfaceComponent : interfaceComponents_)
	{
		interfaceComponent->Spawn();
	}
}

void RInterfaceObject::InitInterfaceComponents()
{
	for (auto& interfaceComponent : interfaceComponents_)
	{
		if (!interfaceComponent->isInit_)
		{
			interfaceComponent->Init();
			interfaceComponent->isInit_ = true;
		}
	}
}

void RInterfaceObject::UpdateInterfaceComponents(const float deltaTime)
{
	for (auto& interfaceComponent : interfaceComponents_)
	{
		if (interfaceComponent->GetCanUpdate())
		{
			interfaceComponent->Update(deltaTime);
		}
	}
}
