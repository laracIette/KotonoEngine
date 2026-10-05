#pragma once
#include "generated/SceneVisibilityField.generated.h"
#include <Widget/Widget.h>

#include <SceneVisibility.h>

class WSceneVisibilityField : public WWidget
{
	GENERATED_WSCENEVISIBILITYFIELD()

private:
	using VisibilityChangedCallback = std::function<void(ESceneVisibility, b8)>;

public:
	WSceneVisibilityField(ESceneVisibility field, std::string_view name, b8 isFieldVisible);

protected:
	WidgetPtr Build() override;

private:
	ESceneVisibility field_;
	std::string name_;
	b8 isFieldVisible_;

	WritableProperty(VisibilityChangedCallback, onVisibilityChanged_, OnVisibilityChanged);
};