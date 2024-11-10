#pragma once
#include "MaterialNodeInput.h"
#include "MaterialNodeOutput.h"
#include <vector>
class MaterialNode
{
public:
	MaterialNode(const std::string& name);
	~MaterialNode();

	const std::string& GetName() const;
	const std::vector<MaterialNodeInput*>& GetInputs() const;
	const MaterialNodeOutput* GetOutput() const;

	virtual std::string GetShaderCode() const = 0;

protected:
	std::vector<MaterialNodeInput*> _inputs;

private:
	const std::string _name;
	const MaterialNodeOutput* _output;
};

