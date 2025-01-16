#include "Drawable.h"

Drawable::Drawable() :
    _isDraw(true), 
    _shader(nullptr)
{
}

const bool Drawable::GetIsDraw() const
{
    return _isDraw && _shader;
}

void Drawable::SetIsDraw(bool isDraw)
{
    _isDraw = isDraw;
}

Shader* Drawable::GetShader() const
{
    return _shader;
}

void Drawable::SetShader(Shader* shader)
{
    _shader = shader;
}

const Color& Drawable::GetColor() const
{
    return _color;
}

void Drawable::SetColor(const Color& color)
{
    _color = color;
}

void Drawable::UpdateShader() const
{

}

void Drawable::Draw() const
{

}
