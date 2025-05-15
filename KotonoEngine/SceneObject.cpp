#include "SceneObject.h"
#include <nlohmann/json.hpp>
#include <kotono_framework/log.h>
#include <kotono_framework/Viewport.h>

void TSceneObject::Init()
{
	Base::Init();

	visibility_ = EVisibility::EditorAndGame;
	viewport_ = &WindowViewport;
}

const UTransform& TSceneObject::GetTransform() const
{ 
	return transform_; 
}

UTransform& TSceneObject::GetTransform()
{
	return transform_;
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
		KT_DEBUG_LOG("TSceneObject::SetParent(): couldn't set the parent of '%s' to itself", GetName().c_str());
		return;
	}
	if (parent == parent_)
	{
		KT_DEBUG_LOG("TSceneObject::SetParent(): couldn't set the parent of '%s' to the same", GetName().c_str());
		return;
	}
	parent_ = parent;
	transform_.SetParent(parent_ ? &parent_->transform_ : nullptr, keepTransform);
}

void TSceneObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	json["parent"] = parent_ ? static_cast<std::string>(parent_->GetGuid()) : ""; // ??
	json["transform"]["position"]["x"] = transform_.GetRelativePosition().x;
	json["transform"]["position"]["y"] = transform_.GetRelativePosition().y;
	json["transform"]["position"]["z"] = transform_.GetRelativePosition().z;
	json["transform"]["rotation"]["w"] = transform_.GetRelativeRotation().w;
	json["transform"]["rotation"]["x"] = transform_.GetRelativeRotation().x;
	json["transform"]["rotation"]["y"] = transform_.GetRelativeRotation().y;
	json["transform"]["rotation"]["z"] = transform_.GetRelativeRotation().z;
	json["transform"]["scale"]["x"] = transform_.GetRelativeScale().x;
	json["transform"]["scale"]["y"] = transform_.GetRelativeScale().y;
	json["transform"]["scale"]["z"] = transform_.GetRelativeScale().z;
}

void TSceneObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	// parent
	transform_.SetRelativePosition({
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
	});
}
