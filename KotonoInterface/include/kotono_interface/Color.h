#pragma once
#include "Widget.h"
struct KtRenderable2DProxy;
class WColor : public WWidget
{
public:
	struct ColorSettings
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	WColor(const ColorSettings& colorSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;
	
protected:
	ColorSettings colorSettings_;
	KtRenderable2DProxy* colorProxy_;
};

