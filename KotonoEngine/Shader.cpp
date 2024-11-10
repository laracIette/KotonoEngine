#include "Shader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() :
	_program(0)
{
}

void Shader::Compile(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
	glUseProgram(0);
	glDeleteProgram(_program);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	CompileShader(vertexShader, vertexShaderCode);
	CompileShader(fragmentShader, fragmentShaderCode);

	_program = glCreateProgram();
	glAttachShader(_program, vertexShader);
	glAttachShader(_program, fragmentShader);
	glLinkProgram(_program);

	// Check for linking errors
	GLint success;
	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(_program, 512, nullptr, infoLog);
		std::cerr << "Error: Shader Program linking failed\n" << infoLog << std::endl;
	}

	// Clean up shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(_program);
}

void Shader::Use() const
{
	glUseProgram(_program);
}

void Shader::SetVertexAttributesLayout() const
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (void*)12);
}

void Shader::CompileShader(const GLuint shader, const std::string& shaderCode) const
{
	const char* source = shaderCode.c_str();

	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// CHeck for compilation errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
	}
}

void Shader::SetMatrix4(const std::string& uniformName, const glm::mat4& matrix) const
{
	GLint location;
	if (TryGetUniformLocation(uniformName, location))
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}

const bool Shader::TryGetUniformLocation(const std::string& uniformName, GLint& location) const
{
	location = glGetUniformLocation(_program, uniformName.c_str());
	return location != GL_INVALID_INDEX;
}