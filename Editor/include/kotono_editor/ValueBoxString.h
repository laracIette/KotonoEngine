#pragma once
#include "ValueBox.h"
class WValueBoxString : public WValueBox<std::string>
{
	using WValueBox::WValueBox;

public:
	WWidget* Build() override;
};