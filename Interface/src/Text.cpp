#include "Text.h"
#include "widgets.h"
#include <kotono_graphics/Font.h>

WidgetPtr WText::Build()
{
	if (shouldWrap_)
	{
		UPtr horizontalWrapList{ UCreate<WHorizontalWrapList>{}() };
		horizontalWrapList->SetItemSpacing(spacing_);
		horizontalWrapList->SetRowSpacing(0.0f);
		horizontalWrapList->SetChildren(GetCharacters());
		horizontalWrapList->SetName("Text Horizontal Wrap List");
		textBody_ = horizontalWrapList;
	}
	else
	{
		UPtr row{ UCreate<WRow>{}() };
		row->SetSpacing(spacing_);
		row->SetChildren(GetCharacters());
		row->SetName("Text Row");
		textBody_ = row;
	}

	return textBody_;
}

std::string_view WText::GetText() const
{
	return text_;
}

glm::vec2 const& WText::GetFontSize() const
{
	return fontSize_;
}

f32 WText::GetSpacing() const
{
	return spacing_;
}

b8 WText::GetShouldWrap() const
{
	return shouldWrap_;
}

void WText::SetText(std::string_view text)
{
	text_ = text;
	UpdateTextBody();
}

void WText::SetFontSize(glm::vec2 const& fontSize)
{
	fontSize_ = fontSize;
	UpdateTextBody();
}

void WText::SetSpacing(f32 spacing)
{
	spacing_ = spacing;
	UpdateTextBody();
}

void WText::SetShouldWrap(b8 shouldWrap)
{
	shouldWrap_ = shouldWrap;
}

void WText::UpdateTextBody() const
{
	if (UPtr row{ TryCast<WRow>(textBody_) })
	{
		UAutoDelete<WWidget> const itemListChildren{ row->GetChildren() };
		row->SetChildren(GetCharacters());
	}
	else if (UPtr horizontalWrapList{ TryCast<WHorizontalWrapList>(textBody_) })
	{
		UAutoDelete<WWidget> const itemListChildren{ horizontalWrapList->GetChildren() };
		horizontalWrapList->SetChildren(GetCharacters());
	}
}

WidgetSet WText::GetCharacters() const
{
	WidgetSet result{};

	const UFont font{ "${ENGINE_DIRECTORY}/Graphics/assets/fonts/default" };

	const auto characterPaths{ font.GetTextPaths(text_) };
	result.reserve(characterPaths.size());

	for (const auto& characterPath : characterPaths)
	{
		UPtr image{ UCreate<WImage>{}(characterPath) };
		image->SetName("text image");

		UPtr box{ UCreate<WBox>{}() };
		box->SetSize(fontSize_);
		box->SetChild(image);
		box->SetName("text box");

		result.Add(box);
	}

	return result;
}

#include "generated/Text.generated.inl"
