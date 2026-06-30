#pragma once
#include "generated/Image.generated.h"
#include "Widget.h"
#include <kotono_common/Path.h>
#include <kotono_graphics/DrawCallBuilder.h>
/// Display an image over the widget's bounds
class WImage final : public WWidget
{
	GENERATED_WIMAGE()

public:
	WImage(const UPath& path = "");

public:
	void Remove() override;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(UPath, path_, Path);

	UDrawCallBuilder drawCallBuilder_;
};

