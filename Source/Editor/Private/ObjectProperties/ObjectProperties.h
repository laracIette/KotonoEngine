#pragma once

#include "Widget/Widget.h"

#include "ObjectProperties.generated.h"

class KSceneObject;

class WObjectProperties : public WWidget
{
	GENERATED()

public:
	explicit WObjectProperties(USpan<UObjectPtr<KSceneObject> const> sceneObjects);

protected:
	auto Build() -> WidgetPtr override;

private:
	UList<UObjectPtr<KSceneObject>> sceneObjects_;
};