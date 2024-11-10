#pragma once
#include <GL/glew.h>
#include <vector>
class VertexBufferObject final
{
public:
    VertexBufferObject();
    ~VertexBufferObject();

    void Bind() const;

    template<typename T>
    void SetData(const std::vector<T>& data) const
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
    }

private:
    GLuint _handle;
};
