#pragma once
#include <string>
class MaterialNodeOutput
{
public:
	MaterialNodeOutput(const std::string& type);

	std::string GetValue() const;
};

