#include "SceneObject.h"
#include <nlohmann/json.hpp>
#include "log.h"
#include <kotono_framework/Viewport.h>
#include "SceneComponent.h"

void TSceneObject::Construct()
{
	Base::Construct();

	viewport_ = &WindowViewport;
	rootComponent_ = AddComponent<KSceneComponent>();
}

void TSceneObject::Init()
{
	Base::Init();

	visibility_ = EVisibility::EditorAndGame;
}

const EVisibility TSceneObject::GetVisibility() const
{
	return visibility_;
}

KtViewport* TSceneObject::GetViewport() const
{
	return viewport_;
}

TSceneObject* TSceneObject::GetParent() const
{
	return parent_;
}

KSceneComponent* TSceneObject::GetRootComponent() const
{
	return rootComponent_;
}

void TSceneObject::SetVisibility(const EVisibility visibility)
{
	visibility_ = visibility;
}

void TSceneObject::SetViewport(KtViewport* viewport)
{
	viewport_ = viewport;
}

void TSceneObject::SetParent(TSceneObject* parent, const ECoordinateSpace keepTransform)
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
	if (parent)
	{
		parent->children_.insert(this);
	}
	if (parent_)
	{
		parent_->children_.erase(this);
	}
	parent_ = parent;
	GetRootComponent()->SetParent(parent_ ? parent_->GetRootComponent() : nullptr, keepTransform);
}

void TSceneObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	json["parent"] = parent_ ? static_cast<std::string>(parent_->GetGuid()) : ""; // ??
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

void TSceneObject::AddComponent(KSceneComponent* component)
{
	components_.insert(component);
}
