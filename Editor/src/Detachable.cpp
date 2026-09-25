#include "Detachable.h"

#include <kotono_core/Interface.h>
#include <kotono_core/InterfaceRoot.h>
#include <kotono_interface/widgets.h>
#include <stdexcept>

WidgetPtr WDetachable::Build()
{
	return (
		UCreate<WStack>{}()
		| (
			UCreate<WColor>{}() 
			| Apply(&WColor::SetColor, Colors::White.WithValue(0.01f))
		)
		| (
			column_ = UCreate<WColumn>{}()
			| (
				UCreate<WConstraint>{}()
				| Apply(&WConstraint::SetAxis, EAxis::Vertical)
				| Apply(&WConstraint::SetSize, 20.0f)
				| (
					UCreate<WRow>{}()
					| (
						UCreate<WWrap>{}()
						| (
							UCreate<WCenter>{}()
							| Apply(&WCenter::SetAxis, EAxis::Vertical)
							| (
								UCreate<WText>{ "Name" }()
								| Apply(&WText::SetText, [this]() { return GetName(); })
								| Apply(&WText::SetFontSize, glm::vec2{ 12.0f, 16.0f })
							)
						)
					)
					| (
						UCreate<WSpacer>{}(EAxis::Horizontal)
					)
					| (
						UCreate<WBox>{}()
						| Apply(&WBox::SetSize, glm::vec2{ 16.0f, 16.0f })
						| (
							UCreate<WButton>{}()
							| Apply(&WButton::SetOnClicked, [this]() { Detach(); })
						)
					)
					| (
						UCreate<WBox>{}()
						| Apply(&WBox::SetSize, glm::vec2{ 16.0f, 16.0f })
						| (
							UCreate<WButton>{}()
							| Apply(&WButton::SetOnClicked, [this]() { Delete(); })
							| Apply(&WButton::SetNormalColor, Colors::Red)
						)
					)
				)
			)
			| (
				child_ = UCreate<WText>{}()
				| Apply(&WText::SetText, "Loading...")
			)
		)
	);
}

void WDetachable::SetChild(WidgetPtr const& widget)
{
	if (column_)
	{
		column_->ReplaceChild(child_, widget);
	}

	if (child_)
	{
		child_->Delete();
	}

	child_ = widget;
}

void WDetachable::Detach()
{
	if (TryCast<WInterfaceRoot>(GetParent()))
	{
		return;
	}

	auto* const oldInterface{ GetInterface() };

	if (GetParent())
	{
		GetParent()->Disown(Ptr());
	}

	oldInterface->OpenWidgetInWindow(Ptr(), GetSize());
}

#include "generated/Detachable.generated.inl"
