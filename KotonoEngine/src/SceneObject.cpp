#include "SceneObject.h"
#include <nlohmann/json.hpp>
#include "log.h"
#include <kotono_framework/WindowViewport.h>
#include "SceneComponent.h"
#include "Engine.h"
#include "ObjectManager.h"

TSceneObject::TSceneObject(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	viewport_ = &WindowViewport;
	rootComponent_ = Engine.ObjectManager().Create<KSceneComponent>(Ptr<TSceneObject>());
	AddComponent(rootComponent_);
}

void TSceneObject::Init()
{
	Base::Init();
}

void TSceneObject::Cleanup()
{
	Base::Cleanup();

	for (const auto& component : components_)
	{
		component->Delete();
	}
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

void TSceneObject::SetViewport(KtWindowViewport* viewport)
{
	viewport_ = viewport;
}

void TSceneObject::SetParent(const UPtr<TSceneObject>& parent, const ECoordinateSpace keepTransform)
{
	if (parent == this)
	{
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "TSceneObject::SetParent(): couldn't set the parent of '%s' to itself", GetName().c_str());
		return;
	}
	if (parent == parent_)
	{
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "TSceneObject::SetParent(): couldn't set the parent of '%s' to the same", GetName().c_str());
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
	components_.Add(component);
	component->componentIndex_ = components_.LastIndex();
}

void TSceneObject::RemoveComponent(const UPtr<KSceneComponent>& component)
{
	const size_t index{ component->componentIndex_ };
	if (components_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
	{
		components_[index]->componentIndex_ = index;
	}
}

//void TSceneObject::SerializeTo(nlohmann::json& json) const
//{
//	Base::SerializeTo(json);
//	json["parent"] = parent_ ? static_cast<std::string>(parent_->Guid()) : ""; // ??
//}

//void TSceneObject::DeserializeFrom(const nlohmann::json& json)
//{
//	Base::DeserializeFrom(json);
//	// parent
//	rootComponent_->DeserializeFrom(json["rootComponent_"]);
//}
