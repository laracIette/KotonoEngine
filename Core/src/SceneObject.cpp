#include "SceneObject.h"
#include <nlohmann/json.hpp>
#include <kotono_common/log.h>
#include <kotono_platform/WindowViewport.h>
#include "SceneComponent.h"
#include "ObjectManager.h"

TSceneObject::TSceneObject(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	viewport_ = &WindowViewport;
}

void TSceneObject::Init()
{
	Base::Init();
}

void TSceneObject::Cleanup()
{
	for (int64_t i{ sceneComponents_.LastIndex() }; i >= 0; --i)
	{
		sceneComponents_[i]->Delete();
	}

	Base::Cleanup();
}

void TSceneObject::Update()
{
}

void TSceneObject::UpdateSceneComponents()
{
	for (const auto& sceneComponent : sceneComponents_)
	{
		if (sceneComponent->GetCanUpdate())
		{
			sceneComponent->Update();
		}
	}
}

bool TSceneObject::GetCanUpdate() const
{
	return canUpdate_;
}

KtWindowViewport* TSceneObject::GetViewport() const
{
	return viewport_;
}

const UPtr<TSceneObject>& TSceneObject::GetParent() const
{
	return parent_;
}

const UPtr<KSceneComponent>& TSceneObject::RootComponent() const
{
	return rootComponent_;
}

void TSceneObject::SetCanUpdate(const bool canUpdate)
{
	canUpdate_ = canUpdate;
}

void TSceneObject::SetViewport(KtWindowViewport* viewport)
{
	viewport_ = viewport;
}

void TSceneObject::SetParent(const UPtr<TSceneObject>& parent, const ECoordinateSpace keepTransform)
{
	if (parent == this)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Core.TSceneObject::SetParent()", "couldn't set the parent of '%s' to itself", GetName().c_str());
		return;
	}
	if (parent == parent_)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Core.TSceneObject::SetParent()", "couldn't set the parent of '%s' to the same", GetName().c_str());
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
		parent->children_.Add(Ptr<TSceneObject>());
		childrenIndex_ = parent->children_.LastIndex();
	}
	parent_ = parent;
	RootComponent()->SetParent(parent_ ? parent_->RootComponent() : nullptr, keepTransform);
}

void TSceneObject::AddComponent(const UPtr<KSceneComponent>& component)
{
	if (sceneComponents_.Empty())
	{
		rootComponent_ = component;
	}
	sceneComponents_.Add(component);
	component->componentIndex_ = sceneComponents_.LastIndex();
}

void TSceneObject::RemoveComponent(const UPtr<KSceneComponent>& component)
{
	const size_t index{ component->componentIndex_ };
	if (sceneComponents_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
	{
		sceneComponents_[index]->componentIndex_ = index;
	}
}

void TSceneObject::Serialize() const
{
	Base::Serialize();
}

void TSceneObject::Deserialize()
{
	Base::Deserialize();

	for (const auto& sceneComponent : sceneComponents_)
	{
		sceneComponent->owner_ = Ptr<TSceneObject>();
	}

	for (const auto& sceneObject : children_)
	{
		sceneObject->parent_ = Ptr<TSceneObject>();
	}
}
