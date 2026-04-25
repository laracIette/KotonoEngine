#include "ValueSliderFloat.h"
#include <kotono_interface/widgets.h>

WidgetPtr WValueSliderFloat::Build()
{
	UPtr box{ Create<WBox>{}() };
	box->SetSize({ 300.0f, 60.0f });

	UPtr bg{ Create<WColor>{}() };
	bg->SetColor(Colors::White.WithValue(0.75f));

	UPtr text{ Create<WText>{}() };
	text->SetText(std::to_string(GetValue()));
	text->SetFontSize({ 42.0f, 50.0f });
	text->SetSpacing(-10.0f);

	UChildOwnerTree(box,
		new UChildrenOwnerTree(Create<WStack>{}(), {
			new UWidgetTreeLeaf(bg),
			new UWidgetTreeLeaf(text),
		})
	).Link();

	return box;
}

#include "generated/ValueSliderFloat.generated.inl"
