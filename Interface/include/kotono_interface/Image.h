#pragma once
#include "generated/Image.generated.h"
#include "Widget.h"
#include <kotono_common/Path.h>
class UInterfaceProxy;
/// Display an image over the widget's bounds
class WImage final : public WWidget
{
	GENERATED_WIMAGE()

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

public:
	const UPath& GetPath() const;

	void SetPath(const UPath& path);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	UPath path_;

	UInterfaceProxy* imageProxy_;
};

