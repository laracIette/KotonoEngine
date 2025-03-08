#include "SceneObject.h"

TSceneObject::TSceneObject() :
	_visibility(VISIBILITY_EDITOR_AND_GAME)
{
}

const UTransform& TSceneObject::GetTransform() const
{ 
	return _transform; 
}

UTransform& TSceneObject::GetTransform()
{
	return _transform;
}

const EVisibility TSceneObject::GetVisibility() const
{
	return _visibility;
}

void TSceneObject::SetVisibility(const EVisibility visibility)
{
	_visibility = visibility;
}