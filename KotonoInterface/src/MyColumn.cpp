#include "MyColumn.h"
#include "widgets.h"
#include "log.h"

WMyColumn::WMyColumn(const MyColumnSettings& myColumnSettings) :
	myColumnSettings_(myColumnSettings),
	count_{ 3 }
{
}

WWidget* WMyColumn::Build()
{
	return new WStack({
		.children = {
			new WRow({
				.spacing = 10.0f,
				.children = {
					/*new WStack({
						.children = {
							new WColor({
								.color = KtColor::Red().WithValue(0.5f).WithAlpha(0.8f),
							}),
							new WButton({
								.onPress = [this]() {
									if (count_ > 1) SetState([this]() { --count_; KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p dec %llu", this, count_); });
								},
							}),
						},
					}),*/
					new WStack({
						.children = {
							new WColor({
								.color = KtColor::Red().WithValue(0.5f).WithAlpha(0.8f),
							}),
							new WButton({
								.onPress = [this]() {
									SetState([this]() { ++count_; KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p inc %llu", this, count_); });
									//KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p inc llu", this);
								},
							}),
						},
					}),
				},
			}),

			new WRow({
				.children = GetColumns(),
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
