#pragma once
#include "generated/VisualizerWindowItem.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_graphics/SceneVisibility.h>

class WVisualizerWindowItem : public WWidget
{
	GENERATED_WVISUALIZERWINDOWITEM()

private:
	using VisibilityChangedCallback = std::function<void(ESceneVisibility, b8)>;

public:
	WVisualizerWindowItem(ESceneVisibility field, std::string_view name);

protected:
	WidgetPtr Build() override;

private:
	ESceneVisibility field_;
	std::string name_;
	b8 isFieldVisible_;

	WritableProperty(VisibilityChangedCallback, onVisibilityChanged_, OnVisibilityChanged);
};