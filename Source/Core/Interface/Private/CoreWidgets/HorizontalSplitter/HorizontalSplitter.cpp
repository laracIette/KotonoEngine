#include "CoreWidgets/HorizontalSplitter/HorizontalSplitter.h"

#include "core_widgets.h"

WHorizontalSplitter::WHorizontalSplitter() 
	: startPreviousWidth_{ 0.0f }
	, startPreviousWeight_{ 0.0f }
{
	ChildrenOwner = UCreate<WRow>{}();
}

auto WHorizontalSplitter::MakeSeparator() -> WidgetPtr
{
	UPtr const separator{ UCreate<WConstraint>{}() };
	return (
		separator
		| Apply(&WConstraint::SetAxis, EAxis::Horizontal)
		| Apply(&WConstraint::SetSize, 10.0f)
		| (
			UCreate<WButton>{}()
			| Apply(&WButton::SetTint, Colors::White.WithValue(0.2f))
			| Apply(&WButton::SetOnPressed, [this, separator]() {
				auto const index{ ChildrenOwner->GetChildren().GetIndex(separator) };
				
				auto const precedingView{ ChildrenOwner->GetChildren() | std::views::take(index) | std::views::reverse };
				auto const findPrevious{ std::ranges::find_if(precedingView
					, [](WidgetPtr const& widget) {
						return widget->GetHasExpand(EExpand::Horizontal);
					}
				) };
				
				auto const succeedingView{ ChildrenOwner->GetChildren() | std::views::drop(index + 1) };
				auto const findNext{ std::ranges::find_if(succeedingView
					, [](WidgetPtr const& widget) {
						return widget->GetHasExpand(EExpand::Horizontal);
					}
				) };
				
				previous_ = nullptr;
				next_ = nullptr;
				
				if (findPrevious != precedingView.end())
				{
					previous_ = *findPrevious;
					Check(Throw, previous_, "previous_ is null");
					startPreviousWidth_ = previous_->GetSize().x;
					startPreviousWeight_ = previous_->GetExpandWeight().x;
				}
				if (findNext != succeedingView.end())
				{
					next_ = *findNext;
					Check(Throw, next_, "next_ is null");
				}
			})
			| Apply(&WButton::SetOnDrag, [this](glm::vec2 const&, glm::vec2 const& total) {
				if (!previous_ || !next_)
				{
					Log(Warning, "previous_ or next_ is null");
					return;
				}
				
				auto const expandWeight{ previous_->GetExpandWeight().x + next_->GetExpandWeight().x };
				auto const combinedWidth{ previous_->GetSize().x + next_->GetSize().x };
				
				auto const newWidth{ std::clamp(startPreviousWidth_ + total.x, 1.0f, combinedWidth - 1.0f) };
				auto const newWeight{ newWidth / startPreviousWidth_ * startPreviousWeight_ };
				
				previous_->SetExpandWeight({ newWeight, 1.0f });
				next_->SetExpandWeight({ expandWeight - newWeight, 1.0f });
			})
		)
	);
}

#include "HorizontalSplitter.generated.inl"
