#pragma once
#include "generated/Text.generated.h"
#include "Widget.h"
/// Display a text
class WText final : public WWidget
{
	GENERATED_WTEXT()

protected:
	WidgetPtr Build() override;

public:
	const std::string& GetText() const;
	const glm::vec2& GetFontSize() const;
	float GetSpacing() const;
	bool GetShouldWrap() const;

	void SetText(const std::string& text); // TODO: set state properties?
	void SetFontSize(const glm::vec2& fontSize);
	void SetSpacing(const float spacing);
	void SetShouldWrap(const bool shouldWrap);

private:
	void UpdateTextBody() const;
	WidgetPool GetCharacters() const;

private:
	UPtr<WWidget> textBody_;

	std::string text_;
	glm::vec2 fontSize_;
	float spacing_;
	bool shouldWrap_;
};

