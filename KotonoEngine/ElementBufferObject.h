#pragma once
#include <GL/glew.h>
#include <vector>
class ElementBufferObject final
{
public:
    ElementBufferObject();
    ~ElementBufferObject();

    void Bind() const;

    template<typename T>
    void SetData(const std::vector<T>& data) const
    {
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
    }

private:
    GLuint _handle;
};
