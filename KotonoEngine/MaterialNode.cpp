#include "MaterialNode.h"

MaterialNode::MaterialNode(const std::string& name) :
	_name(name), _inputs(std::vector<MaterialNodeInput*>()), _output(new MaterialNodeOutput("float"))
{
}

MaterialNode::~MaterialNode()
{
	for (MaterialNodeInput* input : _inputs)
	{
		delete input;
	}
	delete _output;
}

const std::string& MaterialNode::GetName() const
{
	return _name;
}

const std::vector<MaterialNodeInput*>& MaterialNode::GetInputs() const
{
	return _inputs;
}

const MaterialNodeOutput* MaterialNode::GetOutput() const
{
	return _output;
}
