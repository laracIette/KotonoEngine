#pragma once
#include "Widget.h"
class WBox : public WWidget
{
public:
	struct BoxSettings
	{
		glm::vec2 size{ 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	WBox(BoxSettings boxSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	BoxSettings boxSettings_;
};

