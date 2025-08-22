#pragma once
#include "Widget.h"
#include <kotono_framework/Color.h>
struct KtRenderable2DProxy;
class WColor : public WWidget
{
	friend class VColorView;

public:
	struct ColorSettings
	{
		/// default = KtColor::White()
		KtColor color{ KtColor::White() };
	};

	/// Fill the widget's bounds with a color
	WColor(const ColorSettings& colorSettings);

	VView* CreateView() override;

	void Destroy() override;
	
protected:
	ColorSettings colorSettings_;
	KtRenderable2DProxy* colorProxy_;
};

