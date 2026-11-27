#include "SceneObject/SceneObject.h"

#include "Scene/Scene.h"
#include "SceneComponent/SceneComponent.h"
#include <Logging/log.h>

KSceneObject::KSceneObject()
	: isInit_{ false }
	, scene_{ nullptr }
	, canUpdate_{ false }
	, dummy_{ 0 }
{
}

KSceneObject::~KSceneObject()
{
	if (GetScene())
	{
		GetScene()->Remove(Ptr());
	}
	
	for (auto const& sceneComponent : sceneComponents_)
	{
		if (sceneComponent)
		{
			sceneComponent->owner_ = nullptr;
			Delete(sceneComponent);
		}
	}

	SetParent(nullptr, ECoordinateSpace::Relative);
}

void KSceneObject::Init()
{
}

void KSceneObject::Update(f32 deltaTime)
{
}

UScene* KSceneObject::GetScene() const
{
	return scene_;
}

UObjectPtr<KSceneComponent> KSceneObject::GetRootComponent() const
{
	return sceneComponents_.IsEmpty() ? nullptr : sceneComponents_[0];
}

void KSceneObject::SetParent(UObjectPtr<KSceneObject> const& parent, ECoordinateSpace keepTransform)
{
	if (parent == parent_)
	{
		return;
	}

	if (parent == Ptr())
	{
		Log(Warning, "couldn't set the parent of {0} to itself", GetName());
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

	if (UObjectPtr const rootComponent{ GetRootComponent() })
	{
		rootComponent->SetParent(parent_ ? parent_->GetRootComponent() : nullptr, keepTransform);
	}
}

void KSceneObject::AddComponent(UObjectPtr<KSceneComponent> const& component)
{
	if (!component)
	{
		Log(Info, "can't add a null scene component");
		return;
	}

	if (UObjectPtr const rootComponent{ GetRootComponent() })
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

void KSceneObject::RemoveComponent(UObjectPtr<KSceneComponent> const& component)
{
	if (!component)
	{
		Log(Info, "can't remove a null scene component");
		return;
	}

	sceneComponents_.Remove(component);

	for (auto const& sceneComponent : sceneComponents_)
	{
		if (sceneComponent->GetParent() == component)
		{
			sceneComponent->SetParent(component->GetParent(), ECoordinateSpace::Relative);
		}
	}
}

void KSceneObject::Deserialize()
{
	Base::Deserialize();

	for (auto const& sceneComponent : sceneComponents_)
	{
		if (sceneComponent)
		{
			sceneComponent->owner_ = Ptr();
		}
	}

	for (auto const& sceneObject : children_)
	{
		if (sceneObject)
		{
			sceneObject->parent_ = Ptr();
		}
	}
}

void KSceneObject::Spawn()
{
	for (auto const& sceneComponent : sceneComponents_)
	{
		if (sceneComponent)
		{
			sceneComponent->Spawn();
		}
	}
}

void KSceneObject::Despawn()
{
	for (auto const& sceneComponent : sceneComponents_)
	{
		if (sceneComponent)
		{
			sceneComponent->Despawn();
		}
	}
}

void KSceneObject::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const
{
	for (auto const& sceneComponent : sceneComponents_)
	{
		if (sceneComponent)
		{
			sceneComponent->PopulateRenderGraph(sceneRenderGraph, visibility);
		}
	}
}

auto KSceneObject::GetRayIntersectSceneComponent(glm::vec3 const& start, glm::vec3 const& direction) const -> UObjectPtr<KSceneComponent>
{
	for (auto const& sceneComponent : sceneComponents_)
	{
		if (!sceneComponent)
		{
			Log(Warning, "sceneComponent is null");
			continue;
		}
		
		if (sceneComponent->GetRayIntersects(start, direction))
		{
			return sceneComponent;
		}
	}
	
	return nullptr;
}

void KSceneObject::InitSceneComponents()
{
	for (auto const& sceneComponent : sceneComponents_)
	{
		if (sceneComponent && !sceneComponent->isInit_)
		{
			sceneComponent->Init();
			sceneComponent->isInit_ = true;
		}
	}
}

void KSceneObject::UpdateSceneComponents(f32 deltaTime)
{
	for (auto const& sceneComponent : sceneComponents_)
	{
		if (sceneComponent->GetCanUpdate())
		{
			sceneComponent->Update(deltaTime);
		}
	}
}

#include "SceneObject.generated.inl"
