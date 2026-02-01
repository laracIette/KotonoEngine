#pragma once
#include <kotono_interface/Widget.h>
class WValueBox : public WWidget
{
public:
	using ValueToStringFunction = std::function<std::string()>;
	using StringToValueFunction = std::function<void(const std::string&)>;

public:
	struct ValueBoxSettings
	{
		/// default = {}
		/// REQUIRED
		ValueToStringFunction valueToString{};
		/// default = {}
		/// REQUIRED
		StringToValueFunction stringToValue{};
	};

	WValueBox(const ValueBoxSettings& valueBoxSettings);

	WWidget* Build() override;

private:
	//std::string value_;
	ValueBoxSettings valueBoxSettings_;
};