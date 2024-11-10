#include "Drawable3D.h"
#include "Transform.h"

Drawable3D::Drawable3D() :
    _transform(new Transform()), 
    _parent(nullptr)
{
}

Drawable3D::~Drawable3D()
{
    delete _transform;
}

const bool Drawable3D::GetIsDraw() const
{
    return Base::GetIsDraw() && (!_parent || _parent->GetIsDraw());
}

Transform* Drawable3D::GetTransform() const
{
    return _transform;
}

Drawable3D* Drawable3D::GetParent() const
{
    return _parent;
}

void Drawable3D::SetParent(Drawable3D* parent)
{
    _parent = parent;

    glm::vec3 location = _transform->GetWorldLocation();
    glm::quat rotation = _transform->GetWorldRotation();
    glm::vec3 scale = _transform->GetWorldScale();

    _transform->SetParent(parent->GetTransform());

    _transform->SetWorldLocation(location);
    _transform->SetWorldRotation(rotation);
    _transform->SetWorldScale(scale);
}
