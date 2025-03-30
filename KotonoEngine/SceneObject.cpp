#include "SceneObject.h"

void TSceneObject::Init()
{
	_visibility = VISIBILITY_EDITOR_AND_GAME;
	_viewport = &WindowViewport;
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

KtViewport* TSceneObject::GetViewport() const
{
	return _viewport;
}

void TSceneObject::SetVisibility(const EVisibility visibility)
{
	_visibility = visibility;
}

void TSceneObject::SetViewport(KtViewport* viewport)
{
	_viewport = viewport;
}
