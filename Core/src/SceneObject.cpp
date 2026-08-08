#include "SceneObject.h"
#include "Scene.h"
#include "SceneComponent.h"
#include <kotono_common/log.h>

TSceneObject::TSceneObject()
{
}

TSceneObject::~TSceneObject()
{
	for (i64 i{ sceneComponents_.LastIndex() }; sceneComponents_.IsValidIndex(i); --i)
	{
		if (UPtr sceneComponent{ sceneComponents_[i] })
		{
			sceneComponent->Delete();
		}
	}

	SetParent(nullptr, ECoordinateSpace::Relative);
}

void TSceneObject::Init()
{
}

void TSceneObject::Update(const float deltaTime)
{
}

UPtr<TSceneObject>& TSceneObject::GetParent()
{
	return parent_;
}

UPtr<KSceneComponent> TSceneObject::GetRootComponent()
{
	return sceneComponents_.empty() ? nullptr : sceneComponents_[0];
}

void TSceneObject::SetParent(const UPtr<TSceneObject>& parent, const ECoordinateSpace keepTransform)
{
	if (!parent && !parent_)
	{
		return;
	}

	if (parent == this)
	{
		KT_LOG(ELogImportanceLevel::High, "Core", "couldn't set the parent of {0} to itself", GetName());
		return;
	}

	if (parent == parent_)
	{
		KT_LOG(ELogImportanceLevel::High, "Core", "couldn't set the parent of {0} to its current parent", GetName());
		return;
	}

	if (parent_)
	{
		const size index{ childrenIndex_ };
		if (parent_->children_.RemoveAt(index) == EPoolRemoveResult::ItemSwappedAndRemoved)
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

	if (UPtr rootComponent{ GetRootComponent() })
	{
		rootComponent->SetParent(parent_ ? parent_->GetRootComponent() : nullptr, keepTransform);
	}
}

void TSceneObject::AddComponent(const UPtr<KSceneComponent>& component)
{
	if (!component)
	{
		KT_LOG(ELogImportanceLevel::High, "Core", "can't add a null scene component");
		return;
	}

	if (UPtr rootComponent{ GetRootComponent() })
	{
		component->SetParent(rootComponent, ECoordinateSpace::Relative);
	}
	else // component becomes root
	{
		component->owner_ = Ptr();
	}
	sceneComponents_.Add(component);
	component->componentIndex_ = sceneComponents_.LastIndex();

	for (const auto& child : component->children_)
	{
		if (child)
		{
			child->owner_ = Ptr();
		}
	}
}

void TSceneObject::RemoveComponent(const UPtr<KSceneComponent>& component)
{
	if (!component)
	{
		KT_LOG(ELogImportanceLevel::High, "Core", "can't remove a null scene component");
		return;
	}

	const size index{ component->componentIndex_ };
	if (sceneComponents_.RemoveAt(index) == EPoolRemoveResult::ItemSwappedAndRemoved)
	{
		sceneComponents_[index]->componentIndex_ = index;
	}

	for (const auto& sceneComponent : sceneComponents_)
	{
		if (sceneComponent->GetParent() == component)
		{
			sceneComponent->SetParent(component->GetParent(), ECoordinateSpace::Relative);
		}
	}
}

void TSceneObject::Deserialize()
{
	Base::Deserialize();

	for (auto& sceneComponent : sceneComponents_)
	{
		if (sceneComponent)
		{
			sceneComponent->owner_ = Ptr();
		}
	}

	for (auto& sceneObject : children_)
	{
		if (sceneObject)
		{
			sceneObject->parent_ = Ptr();
		}
	}
}

void TSceneObject::Spawn()
{
	for (auto& sceneComponent : sceneComponents_)
	{
		if (sceneComponent)
		{
			sceneComponent->Spawn();
		}
	}
}

URenderContext* TSceneObject::GetRenderContext() const
{
	assert(GetScene());
	return GetScene()->GetRenderContext();
}

void TSceneObject::InitSceneComponents()
{
	for (auto& sceneComponent : sceneComponents_)
	{
		if (sceneComponent && !sceneComponent->isInit_)
		{
			sceneComponent->Init();
			sceneComponent->isInit_ = true;
		}
	}
}

void TSceneObject::UpdateSceneComponents(const float deltaTime)
{
	for (auto& sceneComponent : sceneComponents_)
	{
		if (sceneComponent->GetCanUpdate())
		{
			sceneComponent->Update(deltaTime);
		}
	}
}

#include "generated/SceneObject.generated.inl"
