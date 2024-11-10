#pragma once
#include <GL/glew.h>
#include <string>
#include "VertexArraySetup.h"
class ImageTexture final
{
public:
	ImageTexture(const std::string& path);

	void SetShader(const Shader& shader) const;

	const std::string& GetPath() const;

	void Draw() const;

private:
	GLuint _handle;

	const std::string _path;

	const VertexArraySetup _vertexArraySetup;
};

