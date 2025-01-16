#include "Drawable2D.h"
#include "Rect.h"

Drawable2D::Drawable2D() :
    _rect(new Rect()),
    _parent(nullptr),
    _layer(0)
{
}

Drawable2D::~Drawable2D()
{
    delete _rect;
}

void Drawable2D::Update(float deltaTime)
{
    Base::Update(deltaTime);
    _rect->Update(deltaTime);
}

const bool Drawable2D::GetIsDraw() const
{
    return Base::GetIsDraw() && (!_parent || _parent->GetIsDraw());
}

void Drawable2D::Draw() const
{
    Base::Draw();
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

const int Drawable2D::GetLayer() const
{
    return _parent ? _parent->GetLayer() + _layer + 1 : _layer;
}

void Drawable2D::SetLayer(const int layer)
{
    _layer = layer;
}

const glm::mat4 Drawable2D::GetModelMatrix() const
{
    return GetRect()->GetModelMatrix();
}
