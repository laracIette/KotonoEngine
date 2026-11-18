#pragma once
#include <Widget/Widget.h>

#include "ObjectProperties.generated.h"

class KSceneObject;

class WObjectProperties : public WWidget
{
	GENERATED()

public:
	explicit WObjectProperties(std::span<UObjectPtr<KSceneObject> const> sceneObjects);

protected:
	auto Build() -> WidgetPtr override;

private:
	std::vector<UObjectPtr<KSceneObject>> sceneObjects_;
};