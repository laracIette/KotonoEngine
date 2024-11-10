#include "Drawable.h"

Drawable::Drawable() :
    _isDraw(true), _material(nullptr)
{
}

Drawable::~Drawable()
{
}

const bool Drawable::GetIsDraw() const
{
    return _isDraw;
}

void Drawable::SetIsDraw(const bool isDraw)
{
    _isDraw = isDraw;
}

Material* Drawable::GetMaterial() const
{
    return _material;
}

void Drawable::SetMaterial(Material* material)
{
    _material = material;
}

void Drawable::UpdateShader() const
{
    _material->GetShader().Use();
}

void Drawable::Draw() const
{
}
