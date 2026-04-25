#include "Text.h"
#include "widgets.h"
#include <kotono_graphics/Font.h>

WidgetPtr WText::Build()
{
	if (shouldWrap_)
	{
		UPtr horizontalWrapList{ Create<WHorizontalWrapList>{}() };
		horizontalWrapList->SetItemSpacing(spacing_);
		horizontalWrapList->SetRowSpacing(0.0f);
		horizontalWrapList->SetChildren(GetCharacters());
		horizontalWrapList->SetName("Text Horizontal Wrap List");
		textBody_ = horizontalWrapList;
	}
	else
	{
		UPtr row{ Create<WRow>{}() };
		row->SetSpacing(spacing_);
		row->SetChildren(GetCharacters());
		row->SetName("Text Row");
		textBody_ = row;
	}

	return textBody_;
}

const std::string& WText::GetText() const
{
	return text_;
}

const glm::vec2& WText::GetFontSize() const
{
	return fontSize_;
}

float WText::GetSpacing() const
{
	return spacing_;
}

bool WText::GetShouldWrap() const
{
	return shouldWrap_;
}

void WText::SetText(const std::string& text)
{
	text_ = text;
	UpdateTextBody();
}

void WText::SetFontSize(const glm::vec2& fontSize)
{
	fontSize_ = fontSize;
	UpdateTextBody();
}

void WText::SetSpacing(const float spacing)
{
	spacing_ = spacing;
	UpdateTextBody();
}

void WText::SetShouldWrap(const bool shouldWrap)
{
	shouldWrap_ = shouldWrap;
}

void WText::UpdateTextBody() const
{
	if (textBody_)
	{
		const WidgetPool textBodyChildren{ textBody_->GetChildren() };
		textBody_->SetChildren(GetCharacters());
		for (auto& child : textBodyChildren)
		{
			if (child)
			{
				child->Delete();
			}
		}
	}
}

WidgetPool WText::GetCharacters() const
{
	WidgetPool result{};

	const KtFont font("${ENGINE_DIRECTORY}/Graphics/assets/fonts/default");

	const auto characterPaths{ font.GetTextPaths(text_) };
	result.reserve(characterPaths.size());

	for (const auto& characterPath : characterPaths)
	{
		UPtr image{ Create<WImage>{}() };
		image->SetPath(characterPath);
		image->SetName("text image");

		UPtr box{ Create<WBox>{}() };
		box->SetSize(fontSize_);
		box->SetChild(image);
		box->SetName("text box");

		result.Add(box);
	}

	return result;
}

#include "generated/Text.generated.inl"
