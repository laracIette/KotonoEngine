#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject()
{
	glGenBuffers(1, &_handle);
}

ElementBufferObject::~ElementBufferObject()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_handle);
}

void ElementBufferObject::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _handle);
}