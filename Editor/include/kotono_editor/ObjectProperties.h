#pragma once
#include <kotono_interface/Widget.h>
#include <kotono_core/Ptr.h>
class KObject;
class WObjectProperties : public WWidget
{
public:
	WObjectProperties(const UPtr<KObject>& object);

	WWidget* Build() override;
	void Cleanup() override;

private:
	WWidget* BuildMemberWidget(const std::string& type);

private:
	UPtr<KObject> object_;
};