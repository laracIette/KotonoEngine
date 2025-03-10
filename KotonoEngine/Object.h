#pragma once
#include "base_class.h"
#include <string>
class OObject
{
public:
	const std::string& GetName() const;
	void SetName(const std::string& name);

private:
	std::string _name;
};

