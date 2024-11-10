#include "Drawable2D.h"
#include "Rect.h"

Drawable2D::Drawable2D() :
    _rect(new Rect()), 
    _parent(nullptr)
{
}

Drawable2D::~Drawable2D()
{
    delete _rect;
}

const bool Drawable2D::GetIsDraw() const
{
    return Base::GetIsDraw() && (!_parent || _parent->GetIsDraw());
}

Rect* Drawable2D::GetRect() const
{
    return _rect;
}

Drawable2D* Drawable2D::GetParent() const
{
    return _parent;
}

void Drawable2D::SetParent(Drawable2D* parent)
{
    _parent = parent;
    _rect->SetParent(parent->GetRect());
}

void Drawable2D::UpdateShader() const
{
    Base::UpdateShader();

    GetMaterial()->GetShader().SetMatrix4("model", GetRect()->GetModelMatrix());
}

void Drawable2D::Draw() const
{
}
