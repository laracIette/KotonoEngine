#pragma once
#include "View.h"
class WImage;
class VImageView : public VView
{
public:
	VImageView(WImage* image);

	void Build(UBuildSettings buildSettings) override;
};

