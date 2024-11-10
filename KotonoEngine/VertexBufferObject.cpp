#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject()
{
    glGenBuffers(1, &_handle);
}

VertexBufferObject::~VertexBufferObject()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_handle);
}

void VertexBufferObject::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _handle);
}
