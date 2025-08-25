#include "MyColumn.h"
#include "widgets.h"
#include "log.h"

WMyColumn::WMyColumn(const MyColumnSettings& myColumnSettings) :
	myColumnSettings_(myColumnSettings),
	count_{ 2 }
{
}

WWidget* WMyColumn::Build()
{
	return new WStack({
		.children = {
			new WRow({
				.children = GetColumns(),
			}),

			new WRow({
				.children = {
					new WButton({
						.onPress = [this]() {
							SetState([this]() { if (count_ > 1) --count_; });
						},
					}),
					new WButton({
						.onPress = [this]() {
							SetState([this]() { ++count_; });
						},
					}),
				},
			}),
		},
	});
}

std::vector<WWidget*> WMyColumn::GetColumns() const
{
	std::vector<WWidget*> result{};
	result.reserve(count_);
	for (size_t i{ 0 }; i < count_; ++i)
	{
		result.push_back(
			new WPadding({
				.padding = WPadding::Padding::All(16.0f),
				.child = new WColor({
					.color = myColumnSettings_.color,
				}),
			})
		);
	}
	return result;
}
