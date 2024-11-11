#include "MaterialNode.h"

MaterialNode::MaterialNode(const int id) :
	Id(id), 
	Inputs(std::vector<MaterialNodeInput*>()), 
	Output(new MaterialNodeOutput("float"))
{
}

MaterialNode::~MaterialNode()
{
	for (MaterialNodeInput* input : Inputs)
	{
		delete input;
	}
	delete Output;
}
