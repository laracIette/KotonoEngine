#pragma once
#include "Widget.h"
#include <kotono_framework/Color.h>
class WMyColumn : public WWidget
{
public:
	struct MyColumnSettings
	{
		KtColor color;
	};

	WMyColumn(const MyColumnSettings& myColumnSettings);

	WWidget* Build() override;

private:
	MyColumnSettings myColumnSettings_;
	size_t count_;

	std::vector<WWidget*> GetColumns() const;
};

