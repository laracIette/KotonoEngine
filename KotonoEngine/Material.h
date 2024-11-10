#pragma once
#include "MaterialNode.h"
#include "MaterialNodeLink.h"
#include "Shader.h"
class Material final
{
public:
	Material();
	~Material();

	const Shader& GetShader() const;

	void Compile();

private:
	Shader _shader;

	std::vector<MaterialNode*> _nodes;
	std::vector<MaterialNodeLink*> _links;

	const std::string GenerateVertexShaderCode() const;
	const std::string GenerateFragmentShaderCode() const;
};

