#pragma once
#include <Widget/Widget.h>

#include "ObjectProperties.generated.h"

class TSceneObject;

class WObjectProperties : public WWidget
{
	GENERATED()

public:
	explicit WObjectProperties(std::span<UObjectPtr<TSceneObject> const> sceneObjects);

protected:
	auto Build() -> WidgetPtr override;

private:
	std::vector<UObjectPtr<TSceneObject>> sceneObjects_;
};