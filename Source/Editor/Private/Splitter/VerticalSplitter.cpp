#include "VerticalSplitter.h"

#include <core_widgets.h>

WVerticalSplitter::WVerticalSplitter()
	: startPreviousHeight_{ 0.0f }
	, startPreviousWeight_{ 0.0f }
{
	ChildrenOwner = UCreate<WColumn>{}();
}

auto WVerticalSplitter::MakeSeparator() -> WidgetPtr
{
	UPtr const separator{ UCreate<WConstraint>{}() };
	return (
		separator
		| Apply(&WConstraint::SetAxis, EAxis::Vertical)
		| Apply(&WConstraint::SetSize, 10.0f)
		| (
			UCreate<WButton>{}()
			| Apply(&WButton::SetTint, Colors::White.WithValue(0.2f))
			| Apply(&WButton::SetOnPressed, [this, separator]() {
				auto const index{ ChildrenOwner->GetChildren().GetIndex(separator) };
				
				auto const precedingView{ ChildrenOwner->GetChildren() | std::views::take(index) | std::views::reverse };
				auto const findPrevious{ std::ranges::find_if(precedingView
					, [](WidgetPtr const& widget) {
						return widget->GetHasExpand(EExpand::Vertical);
					}
				) };
				
				auto const succeedingView{ ChildrenOwner->GetChildren() | std::views::drop(index + 1) };
				auto const findNext{ std::ranges::find_if(succeedingView
					, [](WidgetPtr const& widget) {
						return widget->GetHasExpand(EExpand::Vertical);
					}
				) };
				
				previous_ = nullptr;
				next_ = nullptr;
				
				if (findPrevious != precedingView.end())
				{
					previous_ = *findPrevious;
					Check(Throw, previous_, "previous_ is null");
					startPreviousHeight_ = previous_->GetSize().y;
					startPreviousWeight_ = previous_->GetExpandWeight().y;
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
				
				auto const expandWeight{ previous_->GetExpandWeight().y + next_->GetExpandWeight().y };
				auto const combinedHeight{ previous_->GetSize().y + next_->GetSize().y };
				
				auto const newHeight{ std::clamp(startPreviousHeight_ + total.y, 1.0f, combinedHeight - 1.0f) };
				auto const newWeight{ newHeight / startPreviousHeight_ * startPreviousWeight_ };
				
				previous_->SetExpandWeight({ 1.0f, newWeight });
				next_->SetExpandWeight({ 1.0f, expandWeight - newWeight });
			})
		)
	);
}

#include "VerticalSplitter.generated.inl"
