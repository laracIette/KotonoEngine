#pragma once
#include "MaterialNodeInput.h"
#include "MaterialNodeOutput.h"
#include <vector>
class MaterialNode
{
public:
	MaterialNode(const int id);
	~MaterialNode();

	const int Id;

	std::vector<MaterialNodeInput*> Inputs;
	MaterialNodeOutput* Output;

	virtual std::string GetShaderCode() const = 0;
};

