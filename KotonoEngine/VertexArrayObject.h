#pragma once
#include "Shader.h"
class VertexArrayObject final
{
public:
    VertexArrayObject();
    ~VertexArrayObject();

    void Bind() const;

    void SetVertexAttributesLayout(const Shader& shader) const;

private:
    GLuint _handle;
};

