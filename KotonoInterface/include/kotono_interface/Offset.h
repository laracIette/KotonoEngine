#pragma once
#include "Widget.h"
class WOffset : public WWidget
{
public:
	struct OffsetSettings
	{
		glm::vec2 size{ 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	WOffset(OffsetSettings offsetSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	OffsetSettings offsetSettings_;
};

