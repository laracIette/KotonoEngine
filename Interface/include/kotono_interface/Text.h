#pragma once
#include "generated/Text.generated.h"
#include <kotono_core/Widget.h>
/// Display a text
class WText final : public WWidget
{
	GENERATED_WTEXT()

protected:
	WidgetPtr Build() override;

public:
	std::string_view GetText() const;
	glm::vec2 const& GetFontSize() const;
	f32 GetSpacing() const;
	b8 GetShouldWrap() const;

	void SetText(std::string_view text);
	void SetFontSize(glm::vec2 const& fontSize);
	void SetSpacing(f32 spacing);
	void SetShouldWrap(b8 shouldWrap);

private:
	void UpdateTextBody() const;
	WidgetSet GetCharacters() const;

private:
	UPtr<WWidget> textBody_;

	std::string text_;
	glm::vec2 fontSize_;
	f32 spacing_;
	b8 shouldWrap_;
};

