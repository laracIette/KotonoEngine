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

void TSceneObject::Update(f32 deltaTime)
{
}

UScene* TSceneObject::GetScene() const
{
	return scene_;
}

UPtr<TSceneObject> const& TSceneObject::GetParent() const
{
	return parent_;
}

UPtr<KSceneComponent> TSceneObject::GetRootComponent() const
{
	return sceneComponents_.empty() ? nullptr : sceneComponents_[0];
}

void TSceneObject::SetParent(UPtr<TSceneObject> const& parent, ECoordinateSpace keepTransform)
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
		parent_->children_.Remove(Ptr());
	}

	parent_ = parent;

	if (parent_)
	{
		parent_->children_.Add(Ptr());
	}

	if (UPtr rootComponent{ GetRootComponent() })
	{
		rootComponent->SetParent(parent_ ? parent_->GetRootComponent() : nullptr, keepTransform);
	}
}

void TSceneObject::AddComponent(UPtr<KSceneComponent> const& component)
{
	if (!component)
	{
		KT_LOG(ELogImportanceLevel::High, "Core", "can't add a null scene component");
		return;
	}

	if (UPtr const rootComponent{ GetRootComponent() })
	{
		component->SetParent(rootComponent, ECoordinateSpace::Relative);
	}
	else // component becomes root
	{
		component->owner_ = Ptr();
	}
	sceneComponents_.Add(component);

	for (auto& child : component->children_)
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

	sceneComponents_.Remove(component);

	for (auto& sceneComponent : sceneComponents_)
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

	GetScene()->AddSpawnedSceneObject(Ptr());
}

void TSceneObject::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const
{
	for (auto const& sceneComponent : sceneComponents_)
	{
		if (sceneComponent)
		{
			sceneComponent->PopulateRenderGraph(sceneRenderGraph);
		}
	}
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

void TSceneObject::UpdateSceneComponents(f32 deltaTime)
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
