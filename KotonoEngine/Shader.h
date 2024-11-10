#pragma once
#include <string>
#include <gl/glew.h>
#include <glm/glm.hpp>
class Shader final
{
public:
	Shader();
	~Shader();

	void Compile(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

	void Use() const;

	void SetVertexAttributesLayout() const;

	// Uniforms setters
	void SetMatrix4(const std::string& uniformName, const glm::mat4& matrix) const;

private:
	GLuint _program;

	void CompileShader(const GLuint shader, const std::string& shaderCode) const;
	
	const bool TryGetUniformLocation(const std::string& uniformName, GLint& location) const;
};

