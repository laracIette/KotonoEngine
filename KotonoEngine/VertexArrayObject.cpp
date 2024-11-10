#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &_handle);
}

VertexArrayObject::~VertexArrayObject()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &_handle);
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(_handle);
}

void VertexArrayObject::SetVertexAttributesLayout(const Shader& shader) const
{
	Bind();
	shader.SetVertexAttributesLayout();
}
