#pragma once
#include "generated/Text.generated.h"
#include "Widget.h"
/// Display a text
class WText final : public WWidget
{
	GENERATED_WTEXT()

public:
	WidgetPtr Build() override;

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

public:
	const std::string& GetText() const;
	const glm::vec2& GetFontSize() const;
	float GetSpacing() const;
	bool GetShouldWrap() const;

	void SetText(const std::string& text);
	void SetFontSize(const glm::vec2& fontSize);
	void SetSpacing(const float spacing);
	void SetShouldWrap(const bool shouldWrap);

private:
	WidgetVector GetCharacters() const;

private:
	std::string text_;
	glm::vec2 fontSize_;
	float spacing_;
	bool shouldWrap_;
};

