#pragma once
#include "Widget.h"
class WBounds : public WWidget
{
public:
	struct BoundsSettings
	{
		glm::vec2 size{ 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	WBounds(BoundsSettings boundsSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	BoundsSettings boundsSettings_;
};

