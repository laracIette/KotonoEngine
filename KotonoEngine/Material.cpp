#include "Material.h"
#include <sstream>
#include "File.h"

Material::Material()
{
}

Material::~Material()
{
	for (MaterialNode* node : _nodes)
	{
		delete node;
	}
	for (MaterialNodeLink* link : _links)
	{
		delete link;
	}
}

const Shader& Material::GetShader() const
{
	return _shader;
}

void Material::Compile(const std::string& vertexShader, const std::string& fragmentShader)
{
	_shader.Compile(File(vertexShader).GetContent(), File(fragmentShader).GetContent());
}

void Material::Compile()
{
	
}

const std::string Material::GenerateVertexShaderCode() const
{
	return "";
}

const std::string Material::GenerateFragmentShaderCode() const
{
	size_t variableIndex(0);

	std::stringstream stream;
	stream << "#version 430 core\n";

	for (MaterialNode* node : _nodes)
	{
		node->Inputs[0]->GetSource();
	}

	return stream.str();
}
