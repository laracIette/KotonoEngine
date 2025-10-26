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

EVisibility TSceneObject::GetVisibility() const
{
	return rootComponent_->GetVisibility();
}

KtWindowViewport* TSceneObject::GetViewport() const
{
	return viewport_;
}

const UPtr<TSceneObject>& TSceneObject::GetParent() const
{
	return parent_;
}

const UPtr<KSceneComponent>& TSceneObject::GetRootComponent() const
{
	return rootComponent_;
}

void TSceneObject::SetVisibility(const EVisibility visibility)
{
	rootComponent_->SetVisibility(visibility);
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
	GetRootComponent()->SetParent(parent_ ? parent_->GetRootComponent() : nullptr, keepTransform);
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

void TSceneObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	json["parent"] = parent_ ? static_cast<std::string>(parent_->Guid()) : ""; // ??
	/*json["transform"]["position"]["x"] = transform_.position.x;
	json["transform"]["position"]["y"] = transform_.position.y;
	json["transform"]["position"]["z"] = transform_.position.z;
	json["transform"]["rotation"]["w"] = transform_.rotation.w;
	json["transform"]["rotation"]["x"] = transform_.rotation.x;
	json["transform"]["rotation"]["y"] = transform_.rotation.y;
	json["transform"]["rotation"]["z"] = transform_.rotation.z;
	json["transform"]["scale"]["x"] = transform_.scale.x;
	json["transform"]["scale"]["y"] = transform_.scale.y;
	json["transform"]["scale"]["z"] = transform_.scale.z;*/
}

void TSceneObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	// parent
	/*transform_.SetRelativePosition({
		json["transform"]["position"]["x"],
		json["transform"]["position"]["y"],
		json["transform"]["position"]["z"]
	});
	transform_.SetRelativeRotation({
		json["transform"]["rotation"]["w"],
		json["transform"]["rotation"]["x"],
		json["transform"]["rotation"]["y"],
		json["transform"]["rotation"]["z"]
	});
	transform_.SetRelativeScale({ 
		json["transform"]["scale"]["x"],
		json["transform"]["scale"]["y"],
		json["transform"]["scale"]["z"] 
	});*/
}
