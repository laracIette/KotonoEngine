#pragma once
#include "Widget.h"
class WText : public WWidget
{
public:
	struct TextSettings
	{
		/// default = ""
		std::string text{ "" };
		/// default = { 50.0f, 60.0f }
		glm::vec2 fontSize{ 50.0f, 60.0f };
		/// default = 0.0f
		float spacing{ 0.0f };
	};

	/// Display a text
	WText(const TextSettings& textSettings);

	WWidget* Build() override;

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

protected:
	TextSettings textSettings_;

private:
	WidgetVector GetCharacters() const;
};

