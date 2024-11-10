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

void Material::Compile()
{
	_shader.Compile(File("image.vert").GetContent(), File("image.frag").GetContent());
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
		node->GetInputs()[0]->GetSource();
	}

	return stream.str();
}
